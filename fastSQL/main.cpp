#include "main.h"
#include <config.h>
#include <lex.h>
#include <string_utils.h>
#include <date.h>

int main()
{
    using namespace std;
    cout << "Hello, World!" << endl;
    auto *acceptor = new Connection_acceptor();
    acceptor->connection_event_loop();

    FastConfig *config = FastConfig::getInstance();
    config->load();

    auto *list = sql_tokenizer("select * from demo where name='我';");

    for (auto &c:*list)
    {
        cout << c << endl;
    }

    Date *d = Date::now();

    cout << d->format("%Y-%m-%d") << endl;
    return 0;
}
