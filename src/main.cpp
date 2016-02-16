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
        db.execDML("create table source(pk_source integer primary key autoincrement, name char(20), fk_pi integer, foreign key(fk_pi) references pi(pk_pi));");
        db.execDML("create table sensor(pk_sensor integer primary key autoincrement, name char(20), type char(20), fk_source integer, foreign key(fk_source) references source(pk_source));");
        db.execDML("create table actuator(pk_actuator integer primary key autoincrement, name char(20), fk_source integer, foreign key(fk_source) references source(pk_source));");
        db.execDML("create table sensor_data(pk_sensor_data integer primary key autoincrement, data char(20), time datetime not null default current_timestamp, fk_sensor integer, foreign key(fk_sensor) references sensor(pk_sensor));");
        db.execDML("create table actuator_data(pk_actuator_data integer primary key autoincrement, data char(20), time datetime not null default current_timestamp, fk_actuator integer, foreign key(fk_actuator) references actuator(pk_actuator));");

        /* Perform insertions */
        int nRows = db.execDML("insert into pi(name) values ('tyr');") 
	+ db.execDML("insert into source(name, fk_pi) values ('dht11', 1);")
	+ db.execDML("insert into source(name, fk_pi) values ('rf', 1);")
	+ db.execDML("insert into source(name, fk_pi) values ('ir', 1);")
	+ db.execDML("insert into sensor(name, type, fk_source) values ('local', 'TEMPERATURE', 1);")
	+ db.execDML("insert into sensor(name, type, fk_source) values ('local', 'HUMIDITY', 1);")
	+ db.execDML("insert into sensor(name, type, fk_source) values ('rf_weather', 'TEMPERATURE', 2);")
	+ db.execDML("insert into sensor(name, type, fk_source) values ('rf_weather', 'HUMIDITY', 2);")
	+ db.execDML("insert into actuator(name, fk_source) values ('rf_button_1', 2);")
	+ db.execDML("insert into actuator(name, fk_source) values ('ir_button_1', 3);")
	+ db.execDML("insert into sensor_data(data, fk_sensor) values ('25', 1);")
	+ db.execDML("insert into sensor_data(data, fk_sensor) values ('31', 2);")
	+ db.execDML("insert into sensor_data(data, fk_sensor) values ('2', 3);")
	+ db.execDML("insert into sensor_data(data, fk_sensor) values ('8', 4);")
	+ db.execDML("insert into actuator_data(data, fk_actuator) values ('1', 1);")
	+ db.execDML("insert into actuator_data(data, fk_actuator) values ('1', 1);")
	+ db.execDML("insert into actuator_data(data, fk_actuator) values ('UP_KEY', 2);");
        std::cout << nRows << " rows inserted" << std::endl;
	std::cout << "primary key : " << db.lastRowId() << std::endl;

	/* Execute a SELECT and display the results */
	CppSQLite3Query sensor_name = db.execQuery("select pk_sensor,name,type from sensor order by name;");
	//int row=1;
        int last_sensor_pk;
        std::string last_sensor_name;
        std::string last_sensor_type;
        while (!sensor_name.eof()){
	    last_sensor_pk = sensor_name.getIntField(0);
	    last_sensor_name = sensor_name.fieldValue(1);
	    last_sensor_type = sensor_name.fieldValue(2);
	    //if(row%2)
	       std::cout << "Driver name : " << last_sensor_name << std::endl;
	    CppSQLite3Buffer bufSQL;
            bufSQL.format("select data from sensor_data where fk_sensor=%d", last_sensor_pk);
            std::string query_result(bufSQL);
	    CppSQLite3Query temperature_data = db.execQuery(query_result.c_str());
            std::string last_temperature_data;
            while (!temperature_data.eof()){
	   	last_temperature_data = temperature_data.fieldValue(0);
		if(last_sensor_type=="TEMPERATURE"){
	            std::cout << "Temperature : " << last_temperature_data  << std::endl;
		} else if(last_sensor_type=="HUMIDITY"){
	            std::cout << "Humidity : " << last_temperature_data  << std::endl;
		} else {
	            std::cout << "Other : " << last_temperature_data  << std::endl;
		}   
            	temperature_data.nextRow();
            }	    	
            sensor_name.nextRow();
	    //row++;
        }

	CppSQLite3Query actuator_name = db.execQuery("select pk_actuator,name from actuator order by name;");
	int last_actuator_pk;
	std::string last_actuator_name;
        while (!actuator_name.eof()){
	    last_actuator_pk = actuator_name.getIntField(0);
	    last_actuator_name = actuator_name.fieldValue(1);
	    std::cout << "Driver name : " << last_actuator_name << std::endl;
	    if(last_actuator_name=="ir_button_1"){
	    	CppSQLite3Buffer buffSQL;
            	buffSQL.format("select data from actuator_data where fk_actuator=%d", last_actuator_pk);
            	std::string query_result(buffSQL);
	    	CppSQLite3Query actuator_data = db.execQuery(query_result.c_str());
            	std::string last_actuator_data;
            	while (!actuator_data.eof()){
	    	   last_actuator_data = actuator_data.fieldValue(0);
	    	   std::cout << "Last input : " << last_actuator_data << std::endl;
            	   actuator_data.nextRow();
            	}
	    } else if (last_actuator_name=="rf_button_1") {
		CppSQLite3Buffer buffSQL;
            	buffSQL.format("select count(data) from actuator_data where fk_actuator=%d", last_actuator_pk);
	    	int nbClicks = db.execScalar(buffSQL);
		std::cout << "Number of clicks : " << nbClicks << std::endl;
	    }
            actuator_name.nextRow();
        }

    } catch (CppSQLite3Exception& e){
        std::cerr << e.errorCode() << ":" << e.errorMessage() << std::endl;
    }

    return 0;
}


