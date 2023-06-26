Array=csvread('thresholding_results.csv');
col1 = Array (1:200, 1);
col2 = Array(1:200,2);
col4 = Array(1:200, 15);
plot(col1,col2)
plot(col1,col2)

%surf(col1, col2, col4, 'edgecolor', 'none');

F = scatteredInterpolant(col1, col2, col4);

min_long = min(col1);
min_lat = min(col2);
max_long = max(col1);
max_lat = max(col2);

proj_long = linspace(min_long, max_long, 100);
proj_lat = linspace(min_lat, max_lat, 100);

[PROJ_LONG, PROJ_LAT] = ndgrid(proj_long, proj_lat);
PROJ_EE = F(PROJ_LONG, PROJ_LAT);

surf(PROJ_LONG, PROJ_LAT, PROJ_EE, 'edgecolor', 'none');