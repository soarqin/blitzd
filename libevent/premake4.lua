project "event"
    kind "StaticLib"
    language "C"
    files { "**.c", "**.h" }
    excludes { "devpoll.c", "epoll.c", "epoll_sub.c", "kqueue.c", "poll.c", "select.c" }
