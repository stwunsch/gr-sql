# GNU Radio module for writing to SQL databases

The module can write data within GNU Radio flowgraphs to MySQL databases using the GNU Radio message system. Possible use-cases are events with time-stamps or samples from a measurement system. Care about the data rate so that the MySQL database can process the input.

In the following, the example setup uses MariaDB run with a Docker image. Any other setup and MySQL database should work as well (but Docker is awesome!).

Run MariaDB with Docker
-----------------------

First download the latest docker image of MariaDB with 'docker pull mariadb'. You have to start the docker service with a command like 'systemctl enable docker.service' (that depends on your system). Then run the latest Docker image with 'docker run --name mariadb_instance_name -e MYSQL_ROOT_PASSWORD=my_password -d mariadb:latest'. See [0] for more information.

You can start and stop this instance from now on with 'docker start mariadb_instance_name' and 'docker stop mariadb_instance_name'.

Setup the MySQL database
------------------------

We'll connect to the database via the IP address. This can be taken from the output of 'docker inspect mariadb_instance_name | grep IPAddress'. Now you can connect to the database as root with 'mysql -u root -p -h <IPAddress>'. The password has been set with MYSQL_ROOT_PASSWORD=my_password.

Next step is creating a database and a table, which matches your desired output data of the GNU Radio flowgraph. Use 'CREATE DATABASE my_db;' to create a new database and 'USE my_db;' to change to it. A new table can be created with 'CREATE TABLE my_table (id int, value float);'. Here, you have to set the desired data types, which matches the message in the GNU Radio flowgraph (more about this later).

Now create a new user with 'CREATE USER my_user' and give him rights to write to the previously created table with 'GRANT INSERT ON my_table TO my_user;'.

Exit the MySQL terminal and reenter as the new user with 'mysql -u my_user -p -h <IPAddress>'. There is no password for the user (so the -p option is actually not necessary), which can be set with 'SET PASSWORD = PASSWORD('my_password');'.

Now, the MySQL database is fully set up to write with gr-sql to it out of a GNU Radio flowgraph.

Writing to the SQL database with GNU Radio
------------------------------------------

References
----------

[0] https://hub.docker.com/_/mariadb/
