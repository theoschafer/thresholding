% Load the PNG image
imageData = imread(['node_focused.png']);

%imageData = flipud(imageData);

% Create a grid for the surface
[Y, X] = meshgrid(1:size(img,2), 1:size(img,1));
Z = zeros(size(X)); % Create a flat surface (you can modify this if needed)

% Create the figure and plot the surface
figure;
surf(X, Y, Z, 'FaceColor', 'texturemap', 'CData', imageData, 'EdgeColor', 'none');

azimuth= 76.2317;
elevation = 83.9612;
view(azimuth, elevation);
