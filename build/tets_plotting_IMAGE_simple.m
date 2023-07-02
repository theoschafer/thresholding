% Load your image
img = imread('im_with_coord_blurred.png');

% Convert the image to grayscale if it isn't already
if size(img,3) == 3
    img = rgb2gray(img);
end

% Normalize image to [0, 1] range if it isn't already
if max(img(:)) > 1
    img = double(img) / 255;
end

% Create x and y coordinates
[y, x] = meshgrid(1:size(img,2), 1:size(img,1));

% Plot the image
figure;
surf(x, y, img, 'EdgeColor', 'none');
view(3); % change to 3D view
xlabel('X'); ylabel('Y'); zlabel('Z (intensity)');

azimuth= 76.2317;
elevation = 83.9612;
view(azimuth, elevation);