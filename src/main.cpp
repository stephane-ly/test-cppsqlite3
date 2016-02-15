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
        db.execDML("create table sensor(pk_sensor integer primary key autoincrement, name char(20), type char(20), fk_source integer, foreign key(fk_source) references source(pk_source));");
        db.execDML("create table actuator(pk_actuator integer primary key autoincrement, name char(20), fk_source integer, foreign key(fk_source) references source(pk_source));");
        db.execDML("create table sensor_data(pk_sensor_data integer primary key autoincrement, data char(20), time datetime not null default current_timestamp, fk_sensor integer, foreign key(fk_sensor) references sensor(pk_sensor));");
        db.execDML("create table actuator_data(pk_actuator_data integer primary key autoincrement, data char(20), time datetime not null default current_timestamp, fk_actuator integer, foreign key(fk_actuator) references actuator(pk_actuator));");

        /* Perform insertions */
        int nRows = db.execDML("insert into pi(name) values ('tyr');") 
	+ db.execDML("insert into source(fk_pi) values (1);")
	+ db.execDML("insert into source(fk_pi) values (1);")
	+ db.execDML("insert into source(fk_pi) values (1);")
	+ db.execDML("insert into sensor(name, type, fk_source) values ('Local', 'TEMPERATURE', 1);")
	+ db.execDML("insert into sensor(name, type, fk_source) values ('Local', 'HUMIDITY', 1);")
	+ db.execDML("insert into sensor(name, type, fk_source) values ('rf_weather_1', 'TEMPERATURE', 2);")
	+ db.execDML("insert into sensor(name, type, fk_source) values ('rf_weather_1', 'HUMIDITY', 2);")
	+ db.execDML("insert into actuator(name, fk_source) values ('rf_button_1', 2);")
	+ db.execDML("insert into actuator(name, fk_source) values ('ir_remote', 3);")
	+ db.execDML("insert into sensor_data(data, fk_sensor) values ('25', 1);")
	+ db.execDML("insert into sensor_data(data, fk_sensor) values ('31', 2);")
	+ db.execDML("insert into sensor_data(data, fk_sensor) values ('25', 3);")
	+ db.execDML("insert into sensor_data(data, fk_sensor) values ('31', 4);")
	+ db.execDML("insert into actuator_data(data, fk_actuator) values ('1', 1);")
	+ db.execDML("insert into actuator_data(data, fk_actuator) values ('UP_KEY', 2);");
        std::cout << nRows << " rows inserted" << std::endl;
	std::cout << "primary key : " << db.lastRowId() << std::endl;

	/* Execute a SELECT and display the results */
	CppSQLite3Query sensor_db = db.execQuery("select name from sensor order by name;");
        std::string last_sensor_name;
        while (!sensor_db.eof()){
	    if(sensor_db.fieldValue(0) != last_sensor_name){
		last_sensor_name = sensor_db.fieldValue(0);
		std::cout << "Driver name : " << last_sensor_name << std::endl;
	        CppSQLite3Query sensor_type = db.execQuery("select pk_sensor,type from sensor order by 1;");
        	std::string last_sensor_value;
                std::string last_sensor_type;
                while (!sensor_type.eof()){
	    	    if(sensor_type.fieldValue(1) != last_sensor_type){
	   	    	last_sensor_value = sensor_type.fieldValue(0);
	   	    	last_sensor_type = sensor_type.fieldValue(1);
			CppSQLite3Query sensor_data = db.execQuery("select data,fk_sensor from sensor_data order by 1;");
                	std::string last_sensor_data;
                	while (!sensor_data.eof()){
	    	   	    if(sensor_data.fieldValue(0) != last_sensor_data && last_sensor_value == sensor_data.fieldValue(1)){
	   	    	        last_sensor_data = sensor_data.fieldValue(0);
	        		std::cout << last_sensor_type << " : " << last_sensor_data  << std::endl;
			    }
            	            sensor_data.nextRow();
			}
		    }
            	    sensor_type.nextRow();
            	}
	    }
            sensor_db.nextRow();
        }
	CppSQLite3Query actuator_db = db.execQuery("select pk_actuator,name from actuator order by name;");
	std::string last_actuator_value;
	std::string last_actuator_name;
        while (!actuator_db.eof()){
	    if(actuator_db.fieldValue(1) != last_actuator_name){
		last_actuator_value = actuator_db.fieldValue(0);
		last_actuator_name = actuator_db.fieldValue(1);
		std::cout << "Driver name : " << last_actuator_name << std::endl;
	    }
	    CppSQLite3Query actuator_data_db = db.execQuery("select data,fk_actuator from actuator_data order by 1;");
            std::string last_actuator_data;
            while (!actuator_data_db.eof()){
	    	if(actuator_data_db.fieldValue(0) != last_actuator_data && last_actuator_value == actuator_data_db.fieldValue(1)){
	    	    last_actuator_data = actuator_data_db.fieldValue(0);
	    	    std::cout << "Last input : " << last_actuator_data << std::endl;
		}
            	actuator_data_db.nextRow();
            }
            actuator_db.nextRow();
        }

    } catch (CppSQLite3Exception& e){
        std::cerr << e.errorCode() << ":" << e.errorMessage() << std::endl;
    }

    return 0;
}


