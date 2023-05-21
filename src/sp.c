#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <pthread.h>
#include <string.h>

#include "../include/graph.h"
#include "../include/helper.h"

// You must keep these two includes in your code.
// They replace <endian.h> and <semaphore.h> to provide compatibility with macOS.
#include "../include/portable_endian.h"
#include "../include/portable_semaphore.h"

int64_t **links;
int64_t *buf;
pthread_mutex_t mutex;
FILE *output_fd;

typedef struct {
    FILE *input_file;
    FILE *output_stream;
    uint8_t nb_threads;
    bool verbose;
} args_t;

typedef struct sourceArgument {
    uint32_t source;
    uint32_t ops;
    graph_t graph;
    bool verbose;
} sourceArg_t;

void usage(char *prog_name) {
    printf("LEPL1503 - Shortest path algorithm - GroupY2\n\n");
    printf("How to run the program :\n\n");
    printf("   %s [-f 'output_file'] [-n 'n_threads'] [-v] 'input_file'\n\n", prog_name);
    printf("   'input_file'        : (required) Path to the instance file representing the graph to process.\n");
    printf("   -f 'output_file'    : (optional) Path to the file that will contain the program result (.txt/.bin/.ntf).   Default : stdout.\n");
    printf("   -n 'n_threads'      : (optional) Number of computation threads executing the shortest path algorithm.   Default : 4.\n");
    printf("   -v                  : (optional) Allow debug messages. If this is not enabled, no debug messages will be displayed, except error messages in case of failure.   Default : false.\n");
}

int parse_args(args_t *args, int argc, char *argv[]) {
    memset(args, 0, sizeof(args_t));

    // Default values for arguments.
    args->nb_threads = 4;
    args->verbose = false;
    args->output_stream = stdout;
    args->input_file = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "hn:vf:")) != -1) {
        switch (opt) {
            case 'n':
                args->nb_threads = atoi(optarg);
                if (args->nb_threads == 0) {
                    fprintf(stderr, "The number of calculation threads must be greater than 0.   Received : %s\n", optarg);
                    return -1;
                }
                break;
            case 'v':
                args->verbose = true;
                break;
            case 'f':
#ifdef LEPL1503_USE_HEADER_INSTANCE
                fprintf(stderr, "You use a header file as the graph instance. This overrides the use of the argument given by -f. Please remove the include from the header if this is not the desired behavior.\n");
                break;
#endif  // LEPL1503_USE_HEADER_INSTANCE
                args->output_stream = fopen(optarg, "w+");
                if (args->output_stream == NULL) {
                    fprintf(stderr, "Impossible to open the output file %s :   %s\n", optarg, strerror(errno));
                    return -1;
                }
                break;
            case '?':
            case 'h':
                usage(argv[0]);
                return 1;
            default:
                usage(argv[0]);
                return -1;
        }
    }

#ifdef LEPL1503_USE_HEADER_INSTANCE
    fprintf(stderr, "You use a header file as the graph instance. This overrides the use of the argument given by -f. Please remove the include from the header if this is not the desired behavior.\n");

#else
    if (optind == argc) {
        fprintf(stderr, "You must provide an instance file as input.\n");
        return -1;
    }

    if (NULL == (args->input_file = fopen(argv[optind], "r"))) {
        fprintf(stderr, "Unable to open the file that contains the graph instance %s :   %s\n", argv[optind], strerror(errno));
        return -1;
    }
#endif  // LEPL1503_USE_HEADER_INSTANCE

    return 0;
}

void *sourceMain(void *argument) {
    /*By Alba Dekens ; Kevin Sanglier
    Either prints or write in a binary file the informations found by the bellman ford algorithm
    @args : void *argument : a pointer towards an sourceArg structure that has been casted into a void *. The structure contains uint32_t source,
            the starting source the thread start with, uint32_t ops, the number of operations the thread must perform, graph_t graph, a structure
            containg the number of nodes, number of links and the informations of the links and bool verbose, a boolean that decides if further
            informations should be printed
    @returns : print in stdout or write the result of bellman_ford, get_max and get_path in a file
    */
    sourceArg_t arg = *((sourceArg_t *)argument);
    int err_lock;

    for (uint32_t k = 0; k < arg.ops; k++) {
        dp dist_path = bellman_ford(arg.graph, arg.source + k, arg.verbose);
        cn max_c = get_max(dist_path.dist, arg.graph.nb_nodes, arg.source + k);
        path_t pth = get_path(arg.source + k, max_c.max_node, dist_path.path, arg.graph.nb_nodes);

        if (output_fd == stdout || output_fd == stderr) {
            // Write the shortest path information to the terminal.

            err_lock = pthread_mutex_lock(&mutex);
            if (err_lock != 0) exit(EXIT_FAILURE);

            printf("Source : %" PRIu32 "\n", arg.source + k);
            printf("Distances : [");
            for (size_t i = 0; i < arg.graph.nb_nodes; i++) {
                if (i != 0) {
                    printf(", ");
                }
                if (dist_path.dist[i] == INT64_MAX) {
                    printf("inf");
                } else {
                    printf("%ld", dist_path.dist[i]);
                }
            }
            printf("]\n");
            printf("\tDestination : %" PRIu32 "\n", max_c.max_node);
            printf("\tCost : %ld\n", max_c.max_cost);
            printf("\tNumber of nodes : %" PRIu32 "\n", pth.p_size);
            printf("\tPath : ");
            uint32_t i = pth.p_size;
            while (i--) {
                printf("%" PRIu32 " ", pth.p[i]);
            }
            printf("\n");
            err_lock = pthread_mutex_unlock(&mutex);
            if (err_lock != 0) exit(EXIT_FAILURE);

        } else {
            // Write the shortest path information to the output file.
            err_lock = pthread_mutex_lock(&mutex);
            if (err_lock != 0) exit(EXIT_FAILURE);
            // printf("id : %d, %d %d %ld %d\n", arg.source, arg.source+k, max_c.max_node, max_c.max_cost, pth.p_size);
            uint32_t w_source = htobe32(arg.source + k);
            fwrite(&w_source, sizeof(w_source), 1, output_fd);
            uint32_t w_max_node = htobe32(max_c.max_node);
            fwrite(&w_max_node, sizeof(w_max_node), 1, output_fd);
            int64_t w_max_cost = htobe64(max_c.max_cost);
            fwrite(&w_max_cost, sizeof(w_max_cost), 1, output_fd);
            uint32_t w_p_size = htobe32(pth.p_size);
            fwrite(&w_p_size, sizeof(w_p_size), 1, output_fd);
            uint32_t i = pth.p_size;
            while (i--) {
                uint32_t w_p = htobe32(pth.p[i]);
                fwrite(&w_p, sizeof(w_p), 1, output_fd);
            }

            err_lock = pthread_mutex_unlock(&mutex);
            if (err_lock != 0) exit(EXIT_FAILURE);
        }
        // Free the pointers.
        free(pth.p);
        free(dist_path.dist);
        free(dist_path.path);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    args_t args;
    int err = parse_args(&args, argc, argv);
    if (err == -1) {
        exit(EXIT_FAILURE);
    } else if (err == 1) {
        exit(EXIT_SUCCESS);
    }

    output_fd = args.output_stream;
    FILE *fd = args.input_file;

    graph_t graph = read_infos(args.input_file);

    if (args.verbose) {
        printf("Number of nodes : %" PRIu32 " , number of links : %" PRIu32 "\n", graph.nb_nodes, graph.nb_links);
    }

    fseek(fd, 0, SEEK_END);
    int length = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    int64_t *buf = malloc(length * (length / sizeof(int64_t)));

    size_t ret = fread(buf, length, length / sizeof(int64_t), fd);
    if (ret == -1) {
        exit(EXIT_FAILURE);
    }

    links = (int64_t **)malloc(graph.nb_links * sizeof(int64_t));

    pthread_t thread[args.nb_threads];
    int err_thread;

    thread_t arg_arr[args.nb_threads];

    int ops = graph.nb_links;
    int th = args.nb_threads;
    int links_index = 0;
    int temp = graph.nb_links;
    int first = 0;
    for (int i = 0; i < args.nb_threads; i++) {
        ops = ops / th;
        thread_t arg = {buf, length, i, ops, links_index, first};
        first += ops * 3;
        links_index += ops;
        th--;
        ops = temp - ops;
        temp = ops;
        arg_arr[i] = arg;
    }

    for (int i = 0; i < args.nb_threads; i++) {
        err_thread = pthread_create(&(thread[i]), NULL, &read_graph, (void *)&arg_arr[i]);
        if (err_thread != 0) {
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < args.nb_threads; i++) {
        err_thread = pthread_join(thread[i], NULL);
        if (err_thread != 0) {
            exit(EXIT_FAILURE);
        }
    }

    graph.links = links;

    uint32_t nb_nodes = graph.nb_nodes;

    if (args.verbose) {
        // Display of links.
        for (int i = 0; i < graph.nb_links; ++i) {
            printf("Link : %" PRIu32 " -> %" PRIu32 " (cost of %" PRIi32 ")\n", (uint32_t)graph.links[i][0], (uint32_t)graph.links[i][1], (int32_t)graph.links[i][2]);
        }
    }

    if (output_fd == stdout || output_fd == stderr) {
        printf("Number of nodes : %" PRIu32 "\n", nb_nodes);
    } else {
        uint32_t w_nb_nodes = htobe32(nb_nodes);
        fwrite(&w_nb_nodes, sizeof(w_nb_nodes), 1, output_fd);
    }

    int err_mutex = pthread_mutex_init(&mutex, NULL);
    if (err_mutex != 0) {
        exit(EXIT_FAILURE);
    }

    sourceArg_t sourceArgArray[args.nb_threads];

    ops = graph.nb_nodes;
    th = args.nb_threads;
    temp = graph.nb_nodes;
    first = 0;
    int k = 0;
    for (uint32_t i = 0; i < args.nb_threads; i++) {
        if (th > 0 && first <= graph.nb_nodes - 1) {
            ops = ops / th;
            sourceArg_t sourceArg = {first, ops, graph, args.verbose};
            sourceArgArray[k] = sourceArg;
            first += ops;
            th--;
            ops = temp - ops;
            temp = ops;
            k++;
        }
    }

    for (int i = 0; i < args.nb_threads; i++) {
        err_thread = pthread_create(&(thread[i]), NULL, &sourceMain, (void *)&sourceArgArray[i]);
        if (err_thread != 0) {
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < args.nb_threads; i++) {
        err_thread = pthread_join(thread[i], NULL);
        if (err_thread != 0) {
            exit(EXIT_FAILURE);
        }
    }

    // Free the links.
    for (int i = 0; i < graph.nb_links; i++) {
        free(graph.links[i]);
    }

    free(graph.links);
    free(buf);

    if (args.input_file != NULL) {
        fclose(args.input_file);
    }

    if (args.output_stream != stdout) {
        fclose(args.output_stream);
    }
    return 0;
}