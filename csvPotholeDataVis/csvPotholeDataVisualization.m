%{
Array=csvread('bumpy_sensor_data_2023-03-17-18-40-01.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
plot(col1, col2);
title('The Pothole Protector Bumpy Road Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Acceleration');
set(gca, 'XLim', [0,423], 'YLim', [0,400]);
%}

%{
Array=csvread('downhill_sensor_data_2023-03-17-19-22-39.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
plot(col1, col2);
title('The Pothole Protector Downhill Road Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Acceleration');
set(gca, 'XLim', [0,300], 'YLim', [0,670]);
%}

%{
Array=csvread('flatroad_sensor_data_2023-03-17-18-36-14.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
plot(col1, col2);
title('The Pothole Protector Flat Road Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Acceleration');
set(gca, 'XLim', [0,175], 'YLim', [0,400]);
%}

%{
Array=csvread('pothole1_sensor_data_2023-03-17-18-44-36.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
plot(col1, col2);
title('The Pothole Protector Pothole1 Test Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Acceleration');
set(gca, 'XLim', [0,291], 'YLim', [-50,600]);
%}

%{
Array=csvread('pothole2_sensor_data_2023-03-17-18-46-14.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
plot(col1, col2);
title('The Pothole Protector Pothole2 Test Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Acceleration');
set(gca, 'XLim', [0,263], 'YLim', [0,400]);
%}

%{
Array=csvread('pothole3_sensor_data_2023-03-17-18-49-12.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
plot(col1, col2);
title('The Pothole Protector Pothole3 Test Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Acceleration');
set(gca, 'XLim', [0,671], 'YLim', [-20,600]);
%}

%{
Array=csvread('uphill_sensor_data_2023-03-17-18-38-13.csv');
col1 = Array(:, 1);
col2 = Array(:, 2);
plot(col1, col2);
title('The Pothole Protector Uphill Road Acceleration Data');
xlabel('Data Acquisition Point (20 Readings/Second)');
ylabel('Acceleration');
set(gca, 'XLim', [0,256], 'YLim', [0,400]); 
%}


