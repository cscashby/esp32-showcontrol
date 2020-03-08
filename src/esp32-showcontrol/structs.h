#ifndef structs_h
#define structs_h

struct SCOSCMessage {
  String command;
  bool has_arg_int;
  int arg_int;
  bool wait_for_heartbeat;
};

#endif //structs_h
