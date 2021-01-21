#include "main.h"
#include <config.h>
#include <utils/date.h>

int main()
{
    using namespace std;
    cout << "Hello, World!" << endl;
    auto *acceptor = new ConnectionAcceptor();
    acceptor->connectionEventLoop();

    FastConfig *config = FastConfig::getInstance();
    config->load();

    Date *d = Date::now();

    cout << d->format("%Y-%m-%d %H:%M:%S") << endl;
    return 0;
}
