#include <stdlib.h>
#include <stdio.h>
#include <ev.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/fcntl.h> // fcntl
#include <unistd.h> // close

// TODO
// disconnect and reconnect on each newline

#define EV_B data->loop
#define EV_B_ EV_B,
#define SETUP_CB client_t *data = ev_userdata(ev_default_loop(0))

typedef struct {
  EV_P;
  ev_io  stdin_watcher;
  ev_io  remote_w;
  ev_io  send_w;
  int    remote_fd;
  char*  line;
  size_t len;
} client_t;

static void send_cb (EV_P_ ev_io *w, int revents)
{
  SETUP_CB;
  if (revents & EV_WRITE)
  {
    puts ("remote ready for writing...");

    if (-1 == send(data->remote_fd, data->line, data->len, 0)) {
      perror("echo send");
      exit(EXIT_FAILURE);
    }

    // once the data is sent, stop notifications that
    // data can be sent until there is actually more 
    // data to send
    ev_io_stop(EV_B_ &data->send_w);
    ev_io_set(&data->send_w, data->remote_fd, EV_READ);
    ev_io_start(EV_B_ &data->send_w);
  }
  else if (revents & EV_READ)
  {
    int n;
    char str[100] = ".\0";

    printf("[r,remote]");
    n = recv(data->remote_fd, str, 100, 0);
    if (n <= 0) {
      if (0 == n) {
        // an orderly disconnect
        puts("orderly disconnect");
  // initialise an io watcher, then start it
  // this one will watch for stdin to become readable
        ev_io_stop(EV_B_ &data->send_w);
        close(data->remote_fd);
      }  else if (EAGAIN == errno) {
        puts("should never get in this state with libev");
      } else {
        perror("recv");
      }
      return;
    }
    printf("socket client said: %s", str);

  }
}

static void stdin_cb (EV_P_ ev_io *w, int revents)
{
  int len2; // not sure if this is at all useful
  SETUP_CB;

  puts ("stdin written to, reading...");
  len2 = getline(&data->line, &data->len, stdin);
  ev_io_stop(EV_B_ &data->send_w);
  ev_io_set (&data->send_w, data->remote_fd, EV_READ | EV_WRITE);
  ev_io_start(EV_B_ &data->send_w);
  // initialise an io watcher, then start it
  // this one will watch for stdin to become readable
}

static void remote_cb (EV_P_ ev_io *w, int revents)
  // initialise an io watcher, then start it
  // this one will watch for stdin to become readable
{
  SETUP_CB;
  puts ("connected, now watching stdin");
  // Once the connection is established, listen to stdin
  ev_io_start(EV_B_ &data->stdin_watcher);
  // Once we're connected, that's the end of that
  ev_io_stop(EV_B_ &data->remote_w);
}


// Simply adds O_NONBLOCK to the file descriptor of choice
int setnonblock(int fd)
{
  int flags;

  flags = fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;
  return fcntl(fd, F_SETFL, flags);
}

static void connection_new(EV_P_ char* sock_path) {
  int len;
  struct sockaddr_un remote;
  SETUP_CB;

  if (-1 == (data->remote_fd = socket(AF_UNIX, SOCK_STREAM, 0))) {
      perror("socket");
      exit(1);
  }

  // Set it non-blocking
  if (-1 == setnonblock(data->remote_fd)) {
    perror("echo client socket nonblock");
    exit(EXIT_FAILURE);
  }

  // this should be initialized before the connect() so
  // that no packets are dropped when initially sent?
  // http://cr.yp.to/docs/connect.html

  // initialize the connect callback so that it starts the stdin asap
  ev_io_init (&data->remote_w, remote_cb, data->remote_fd, EV_WRITE);
  ev_io_start(EV_B_ &data->remote_w);
  // initialize the send callback, but wait to start until there is data to writecl
  ev_io_init(&data->send_w, send_cb, data->remote_fd, EV_READ);
  ev_io_start(EV_B_ &data->send_w);

  remote.sun_family = AF_UNIX;
  strcpy(remote.sun_path, sock_path);
  len = strlen(remote.sun_path) + sizeof(remote.sun_family);

  if (-1 == connect(data->remote_fd, (struct sockaddr *)&remote, len)) {
      perror("connect");
      
      // Disable the callbacks;
      ev_io_stop(EV_B_ &data->remote_w);
      ev_io_stop(EV_B_ &data->send_w);
  }
}

void print_backend(int backend)
{
  switch(backend)
  {
    case EVBACKEND_SELECT:
      puts("select");
      break;
    case EVBACKEND_POLL:
      puts("poll");
      break;
    case EVBACKEND_EPOLL:
      puts("epoll");
      break;
    case EVBACKEND_KQUEUE:
      puts("kqueue");
      break;
    case EVBACKEND_DEVPOLL:
      puts("/dev/poll");
      break;
    case EVBACKEND_PORT:
      puts("port");
      break;
    default:
      puts("other");
      break;
  }
}


int main (void)
{
  client_t *data = calloc(sizeof(client_t), 1);
  data->loop = EV_DEFAULT;

  // Register our custom struct with libev
  ev_set_userdata(EV_B_ data);

  printf("Running Unix client, libev v%d.%d\nUsing backend: ",
      ev_version_major(), ev_version_minor());
  print_backend(ev_backend(EV_B));

  // initialise an io watcher, then start it
  // this one will watch for stdin to become readable
  setnonblock(0);
  ev_io_init(&data->stdin_watcher, stdin_cb, fileno(stdin), EV_READ);

  connection_new(EV_B_ "/tmp/libev-echo.sock");

  // now wait for events to arrive
  ev_loop(EV_B_ 0);

  // unloop was called, so exit
  free(data);
  return 0;
}

