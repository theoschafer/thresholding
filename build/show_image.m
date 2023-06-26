% Load the PNG image
imageData = imread(['mit_sp_boat_zoom_blur.png']);

imageData = flipud(imageData);

% Create a grid for the surface
[X, Y] = meshgrid(linspace(-1, 1, size(imageData, 2)), linspace(-1, 1, size(imageData, 1)));
Z = zeros(size(X)); % Create a flat surface (you can modify this if needed)

% Create the figure and plot the surface
figure;
surf(X, Y, Z, 'FaceColor', 'texturemap', 'CData', imageData, 'EdgeColor', 'none');

azimuth= 76.2317;
elevation = 83.9612;
view(azimuth, elevation);