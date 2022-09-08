#include "tmessage.h"

#include "types/scriptable.h"

MessageHandler *MessageHandler::singleton;

//=========================================================================
// TMessage
//=========================================================================

TMessage::TMessage(String p_description, const String &p_file_name,
                   int p_line_number) {
    description = p_description;
    file_name = p_file_name;
    line_number = p_line_number;
    count = 0;
}

void TMessage::log() { ERROR_HANDLER->Log(*this); }

//=========================================================================
// TError
//=========================================================================

TError::TError(const String &p_description, const String &p_file_name,
               int p_line_number)
    : TMessage(p_description, p_file_name, p_line_number) {
    type = T_ERROR;
    log();
}

//=========================================================================
// TWarning
//=========================================================================

TWarning::TWarning(const String &p_description, const String &p_file_name,
                   int p_line_number)
    : TMessage(p_description, p_file_name, p_line_number) {
    type = T_WARNING;
    log();
}

//=========================================================================
// TLog
//=========================================================================

TLog::TLog(const String &p_description, const String &p_file_name,
           int p_line_number)
    : TMessage(p_description, p_file_name, p_line_number) {
    type = T_LOG;
    log();
}

//=========================================================================
// TInfo
//=========================================================================

TInfo::TInfo(const String &p_description, const String &p_file_name,
             int p_line_number)
    : TMessage(p_description, p_file_name, p_line_number) {
    type = T_INFO;
    log();
}

//=========================================================================
// ErrorHandler
//=========================================================================

MessageHandler::MessageHandler() {
    signal = nullptr;

    messages = Vector<TMessage>();
    filters = Array<char>();
    complete_description = false;
}

void MessageHandler::Log(const TMessage &p_message) {
    TMessage *msg = new TMessage(p_message);

    for (int c = 0; c < messages.size(); c++) {
        if (messages[c]->description == msg->description) {
            messages[c]->count++;
            return;
        }
    }

    messages.push_back(msg);

    emit(messages.size() - 1);

    if (!filters[msg->type]) return;

    if (complete_description) {
        switch (msg->type) {
            case TMessage::T_ERROR:
                std::cout << "ERROR: " << msg->description
                          << ", file: " << msg->file_name
                          << ", line: " << msg->line_number << std::endl;
                break;
            case TMessage::T_WARNING:
                std::cout << "WARNING: " << msg->description
                          << ", file: " << msg->file_name
                          << ", line: " << msg->line_number << std::endl;
                break;
            case TMessage::T_INFO:
                std::cout << "INFO: " << msg->description
                          << ", file: " << msg->file_name
                          << ", line: " << msg->line_number << std::endl;
                break;
            case TMessage::T_LOG:
                std::cout << "LOG: " << msg->description
                          << ", file: " << msg->file_name
                          << ", line: " << msg->line_number << std::endl;
                break;
        }
    } else {
        switch (msg->type) {
            case TMessage::T_ERROR:
                std::cout << "ERROR: " << msg->description << std::endl;
                break;
            case TMessage::T_WARNING:
                std::cout << "WARNING: " << msg->description << std::endl;
                break;
            case TMessage::T_INFO:
                std::cout << "INFO: " << msg->description << std::endl;
                break;
            case TMessage::T_LOG:
                std::cout << "LOG: " << msg->description << std::endl;
                break;
        }
    }
}
void MessageHandler::clean() { messages.clean(); }

void MessageHandler::init() {
    singleton = new MessageHandler;

    singleton->filters.push_back(true);  // UNDEF
    singleton->filters.push_back(true);  // T_ERROR
    singleton->filters.push_back(true);  // WARNING
    singleton->filters.push_back(true);  // LOG
    singleton->filters.push_back(true);  // INFO
}

void MessageHandler::bind_methods() {
    // REG_SINGLETON(singleton);

    // REG_SIGNAL("logged");
}

TMessage *MessageHandler::get_message(int p_index) const {
    return messages[p_index];
}

void MessageHandler::emit(int p_index) {
    if (!signal) return;

    for (Connection &connection : signal->connections) {
        Connection *c = const_cast<Connection *>(&connection);

        if (!connection.method) return;

        if (connection.type == Connection::NATIVE) {
            if (connection.method->arg_count == 2) {
                V_Method_2 *m =
                    reinterpret_cast<V_Method_2 *>(connection.method);
                m->operator()(connection.object, p_index);
            }
        } else if (connection.type == Connection::TITANSCRIPT)
            connection.scriptable->run(connection.name, Arguments(p_index));
    }
}

void MessageHandler::connect(Object *p_object, const String &p_method) {
    Connection connection;
    connection.register_native_method(p_object, p_method);

    signal = new Signal;
    signal->name = "logged";
    signal->attach_connection(connection);
}

MessageHandler *MessageHandler::get_singleton() { return singleton; }
