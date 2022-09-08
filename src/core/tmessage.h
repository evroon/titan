#pragma once

#include <iostream>

#include "array.h"
#include "core/string.h"
#include "vector.h"

#define T_ERROR(X) TError(X, __FILE__, __LINE__)
#define T_WARNING(X) TWarning(X, __FILE__, __LINE__)
#define T_LOG(X) TLog(X, __FILE__, __LINE__)
#define T_INFO(X) TInfo(X, __FILE__, __LINE__)

#define ERROR_HANDLER MessageHandler::get_singleton()

struct TMessage {
    enum Type { UNDEF, T_ERROR, T_WARNING, T_LOG, T_INFO };

    Type type;
    String description;
    String file_name;
    int line_number;
    int count;

    TMessage(String p_description, const String& p_file_name,
             int p_line_number);

   protected:
    void log();
};

struct TError : TMessage {
    TError(const String& p_description, const String& p_file_name,
           int p_line_number);
};

struct TWarning : TMessage {
    TWarning(const String& p_description, const String& p_file_name,
             int p_line_number);
};

struct TLog : TMessage {
    TLog(const String& p_description, const String& p_file_name,
         int p_line_number);
};

struct TInfo : TMessage {
    TInfo(const String& p_description, const String& p_file_name,
          int p_line_number);
};

class Object;
class Signal;

class MessageHandler {
    MessageHandler();

   public:
    void Log(const TMessage& p_message);
    void clean();

    TMessage* get_message(int p_index) const;

    void emit(int p_index);

    void connect(Object* p_object, const String& p_method);

    static MessageHandler* get_singleton();

    static void init();
    static void bind_methods();

   private:
    Signal* signal;

    Vector<TMessage> messages;
    Array<char> filters;
    bool complete_description;

    static MessageHandler* singleton;
};
