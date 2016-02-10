#include <ctime>
#include <iostream>

#include "CppSQLite3.h"

int main(int, char**){
    try {
        /* Remove the database (THIS WONT BE IN A FINAL APPLICATION) */

        remove("test.db");

        /* Create the database object */

        CppSQLite3DB db;

        /* Print the SQLite version */

        std::cout << "SQLite Version: " << db.SQLiteVersion() << std::endl;

        /* Open (connect) the database */

        db.open("test.db");

        /* Create tables */

        db.execDML("create table pi(pk_pi integer primary key autoincrement, name char(20));");
        db.execDML("create table source(pk_source integer primary key autoincrement, fk_pi integer, foreign key(fk_pi) references pi(pk_pi));");
        db.execDML("create table sensor(pk_sensor integer primary key autoincrement, type char(20), name char(20), fk_source integer, foreign key(fk_source) references source(pk_source));");
        db.execDML("create table actuator(pk_actuator integer primary key autoincrement, name char(20), fk_source integer, foreign key(fk_source) references source(pk_source));");
        db.execDML("create table sensor_data(pk_sensor_data integer primary key autoincrement, data char(20), time datetime not null default '0000-00-00 00:00:00', fk_sensor integer, foreign key(fk_sensor) references sensor(pk_sensor));");
        db.execDML("create table actuator_data(pk_actuator_data integer primary key autoincrement, data char(20), time datetime not null default '0000-00-00 00:00:00', fk_actuator integer, foreign key(fk_actuator) references actuator(pk_actuator));");

        /* Perform insertions */

        int nRows = db.execDML("insert into pi(name) values ('tyr');") 
	+ db.execDML("insert into source(fk_pi) values (1);")
	+ db.execDML("insert into source(fk_pi) values (1);")
	+ db.execDML("insert into source(fk_pi) values (1);")
	+ db.execDML("insert into sensor(type, name, fk_source) values ('TEMPERATURE', 'Local', 1);")
	+ db.execDML("insert into sensor(type, name, fk_source) values ('HUMIDITY', 'Local', 1);")
	+ db.execDML("insert into sensor(type, name, fk_source) values ('TEMPERATURE', 'rf_weather_1', 2);")
	+ db.execDML("insert into sensor(type, name, fk_source) values ('HUMIDITY', 'rf_weather_1', 2);")
	+ db.execDML("insert into actuator(name, fk_source) values ('rf_button_1', 2);");
	+ db.execDML("insert into actuator(name, fk_source) values ('ir_remote', 3);");
        std::cout << nRows << " rows inserted" << std::endl;

    } catch (CppSQLite3Exception& e){
        std::cerr << e.errorCode() << ":" << e.errorMessage() << std::endl;
    }

    return 0;
}
