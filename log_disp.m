% 16.Unified Fall 2013
% Lab 1
% Pressure-Temperature Sensor Datalog Parser

% Enter the name of the file you want to parse here:
filename = 'DATA.CSV';

% Make variables in which to store the data we read from the file.
t = {}; % time
T = {}; % temperature
h = {}; % altitude
P = {}; % pressure

% open the file
fid = fopen(filename);

% Read each line of the file. If a header is encountered, start a new cell.
tline = fgetl(fid);
count = 0;
while ischar(tline)
    if tline(1) == 'T'
        % We found a header line. Increment the cell count
        count = count +1;
        t{count} = [];
        T{count} = [];
        h{count} = [];
        P{count} = [];
    else
        % We found a data line. Append the data.
        c = textscan( tline, '%f,%f,%f,%f' );
        if length(t) == 0
            t = {c{1}};
            T = {c{2}};
            h = {c{3}};
            P = {c{4}};
        else
            t{count} = [t{count}, c{1}];
            T{count} = [T{count}, c{2}];
            h{count} = [h{count}, c{3}];
            P{count} = [P{count}, c{4}];
        end
    end
    tline = fgetl(fid);
end

% close the file
fclose(fid);

% plot the data
for i = 1:count
    figure    
    subplot(3,1,1)
    plot(t{i}, T{i}/10.0)
    ylabel('Temperature [C]')
    subplot(3,1,2)
    plot(t{i}, P{i})
    ylabel('Pressure [Pa]')
    subplot(3,1,3)
    plot(t{i}, h{i}/100.0)
    ylabel('Altitude [m]')
    xlabel('Time [ms]')
end