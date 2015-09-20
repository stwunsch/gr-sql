# SQL setup

Use 'sudo apt-get install mariadb-client' package.

Run 'sudo mysql' to open the sql terminal.

Create new database with 'CREATE DATABASE my_db;' and change to database with 'USE my_db;'.

Create table which suits the gnuradio sql block input, e.g., 'CREATE TABLE my_table (id int, value float);'.

Create new user with 'CREATE USER my_user'.

Grant INSERT rights to my_user with 'GRANT INSERT ON my_table TO my_user;'.

Change to new user closing sql terminal and reopen with 'mysql -u my_user'.

Set a password for this user with 'SET PASSWORD = PASSWORD('my_password');'. Now you have to login with 'mysql -u my_user -p'! You can disable the password by running 'SET PASSWORD = PASSWORD('');'.

# Writing to the SQL database with GNU Radio


