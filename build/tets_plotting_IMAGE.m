
%dilated_Image_t60e3_(blurred)_inverted.png
%im_with_coord.png

% Load your grayscale image for 3D plotting
img = imread('dilated_Image_t60e3_(blurred)_inverted.png');

% Convert the image to grayscale if it isn't already
if size(img,3) == 3
    img = rgb2gray(img);
end

% Normalize image to [0, 1] range if it isn't already
if max(img(:)) > 1
    img = double(img) / 255;
end

% Load your color image for 2D plotting below the 3D plot
img_color = imread('im_with_coord.png');

% Create x and y coordinates
[y, x] = meshgrid(1:size(img,2), 1:size(img,1));

% Create z coordinate for the floating image. Adjust the offset as needed.
z_offset = -15;  % Modify this value to adjust vertical spacing
z_image = z_offset * ones(size(img));

% Create the figure
figure;

% Create the first axes for the color image plot
ax1 = axes;
h = surf(ax1, x, y, z_image, 'FaceColor', 'texturemap', 'EdgeColor', 'none');
set(h, 'CData', flip(img_color, 2));  % Apply the color image as texture and flip it
colormap(ax1, 'default'); % use the default colormap for the color plot

% Create the second axes for the 3D grayscale plot
ax2 = axes;
surf(ax2, x, y, img, 'EdgeColor', 'none', 'FaceAlpha', 0.7); % Added 'FaceAlpha' property
colormap(ax2, 'jet'); % use the 'jet' colormap for the grayscale plot

% Hide the second axes and link the two axes for proper navigation
linkprop([ax1, ax2], {'CameraPosition', 'CameraUpVector'});
ax2.Visible = 'off';
ax2.XTick = []; ax2.YTick = []; ax2.ZTick = [];  % Hide tick values

view(3); % change to 3D view
xlabel('X'); ylabel('Y'); zlabel('Z (intensity)');

azimuth= 76.2317;  %type [azimuth, elevation] = view in the command invite
elevation = 83.9612;
view(azimuth, elevation);