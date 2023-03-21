%{
Array=csvread('bumpy_sensor_data_2023-03-17-18-40-01.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
col3 = Array(:, 3);
col4 = Array(:, 4);
hold on
plot(col1, col2, col1, col3);
plot(col1, col4, 'color', 'm');
title('The Pothole Protector Bumpy Road Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Adjusted Acceleration');
set(gca, 'XLim', [0,423], 'YLim', [0,400]);
legend('Sensor Data', 'Average Value', 'Pothole Detected Boolean');
hold off
%}

%{
Array=csvread('downhill_sensor_data_2023-03-17-19-22-39.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
col3 = Array(:, 3);
col4 = Array(:, 4);
hold on
plot(col1, col2, col1, col3);
plot(col1, col4, 'color', 'm');
title('The Pothole Protector Downhill Road Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Adjusted Acceleration');
set(gca, 'XLim', [0,300], 'YLim', [0,670]);
legend('Sensor Data', 'Average Value', 'Pothole Detected Boolean');
hold off
%}

%{
Array=csvread('flatroad_sensor_data_2023-03-17-18-36-14.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
col3 = Array(:, 3);
col4 = Array(:, 4);
hold on
plot(col1, col2, col1, col3);
plot(col1, col4, 'color', 'm');
title('The Pothole Protector Flat Road Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Adjusted Acceleration');
set(gca, 'XLim', [0,175], 'YLim', [0,400]);
legend('Sensor Data', 'Average Value', 'Pothole Detected Boolean');
hold off
%}

%{
Array=csvread('pothole1_sensor_data_2023-03-17-18-44-36.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
col3 = Array(:, 3);
col4 = Array(:, 4);
hold on
plot(col1, col2, col1, col3);
plot(col1, col4, 'color', 'm');
title('The Pothole Protector Pothole1 Test Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Adjusted Acceleration');
set(gca, 'XLim', [0,291], 'YLim', [-50,600]);
legend('Sensor Data', 'Average Value', 'Pothole Detected Boolean');
hold off
%}

%{
Array=csvread('pothole2_sensor_data_2023-03-17-18-46-14.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
col3 = Array(:, 3);
col4 = Array(:, 4);
hold on
plot(col1, col2, col1, col3);
plot(col1, col4, 'color', 'm');
title('The Pothole Protector Pothole2 Test Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Adjusted Acceleration');
set(gca, 'XLim', [0,263], 'YLim', [0,400]);
legend('Sensor Data', 'Average Value', 'Pothole Detected Boolean');
hold off
%}

%{
Array=csvread('pothole3_sensor_data_2023-03-17-18-49-12.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
col3 = Array(:, 3);
col4 = Array(:, 4);
hold on
plot(col1, col2, col1, col3);
plot(col1, col4, 'color', 'm');
title('The Pothole Protector Pothole3 Test Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Adjusted Acceleration');
set(gca, 'XLim', [0,671], 'YLim', [-20,600]);
legend('Sensor Data', 'Average Value', 'Pothole Detected Boolean');
hold off
%}

%{
Array=csvread('uphill_sensor_data_2023-03-17-18-38-13.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
col3 = Array(:, 3);
col4 = Array(:, 4);
hold on
plot(col1, col2, col1, col3);
plot(col1, col4, 'color', 'm');
title('The Pothole Protector Uphill Road Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Adjusted Acceleration');
set(gca, 'XLim', [0,256], 'YLim', [0,400]); 
legend('Sensor Data', 'Average Value', 'Pothole Detected Boolean');
hold off
%}