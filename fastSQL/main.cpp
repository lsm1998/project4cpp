#include "main.h"
#include <config.h>
#include <lex.h>

int main()
{
    using namespace std;
    cout << "Hello, World!" << endl;
    auto *acceptor = new Connection_acceptor();
    acceptor->connection_event_loop();

    FastConfig *config = FastConfig::getInstance();
    config->load();
    config->load();

    sql_tokenizer("select * from demo where name='我';");
    return 0;
}
