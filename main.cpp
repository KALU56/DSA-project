// main.cpp
#include <iostream>
#include <libpq-fe.h>
#include "main.h"

void connectAndQuery() {
    PGconn* conn = PQconnectdb("host=localhost port=5432 dbname=blood_bank user=postgres password=kaluLILUYA#1");

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return;
    }

    PGresult* res = PQexec(conn, "SELECT version();");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "SELECT failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
        return;
    }

    std::cout << "PostgreSQL version: " << PQgetvalue(res, 0, 0) << std::endl;

    PQclear(res);
    PQfinish(conn);
}

int main() {
    connectAndQuery();
    return 0;
}
