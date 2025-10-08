%shamelessly from GPT

clsoe all; clc; clear;
% ===== pick file via dialog =====
[fn, fp] = uigetfile( ...
    {'*.txt;*.log;*.csv','Text Logs (*.txt, *.log, *.csv)'; '*.*','All Files (*.*)'}, ...
    'Select waveform log file');
if isequal(fn,0)
    disp('No file selected. Aborting.');
    return;
end
fname = fullfile(fp, fn);
showLegends = true;

%% ===== 1st pass: count valid lines =====
fid = fopen(fname,'r');
if fid==-1, error('Cannot open file: %s', fname); end
validCount = 0;
expr = 'TIME::\s*([+-]?\d+(\.\d+)?)\s*::\s*PGMTime::\s*([+-]?\d+(\.\d+)?)\s*::\s*SMPL::\s*(.*)$';
while true
    line = fgetl(fid);
    if ~ischar(line), break; end
    if ~isempty(regexp(line, expr, 'once'))
        validCount = validCount + 1;
    end
end
fclose(fid);

if validCount == 0
    error('No valid lines were found in %s.', fname);
end
fprintf('Detected %d valid lines.\n', validCount);

%% ===== Preallocate arrays =====
time = zeros(validCount, 1);
pgm  = zeros(validCount, 1);
smpl = zeros(validCount, 16);

%% ===== 2nd pass: parse values =====
fid = fopen(fname,'r');
if fid==-1, error('Cannot reopen file: %s', fname); end

idx = 0;
while true
    line = fgetl(fid);
    if ~ischar(line), break; end

    tkn = regexp(line, expr, 'tokens','once');
    if isempty(tkn), continue; end

    idx = idx + 1;
    tVal = str2double(tkn{1});
    pVal = str2double(tkn{2});
    rest = strtrim(tkn{3});

    nums = str2double(regexp(rest,'\s+','split'));
    if numel(nums) < 16
        nums(end+1:16) = NaN; % pad short lines
    else
        nums = nums(1:16);
    end

    time(idx) = tVal;
    pgm(idx)  = pVal;
    smpl(idx, :) = nums;
end
fclose(fid);

% Trim arrays in case some lines were malformed
time = time(1:idx);
pgm  = pgm(1:idx);
smpl = smpl(1:idx, :);

%% ===== Convert time to hours =====
time = time / 3600;  % convert seconds → hours

%% ===== Split voltage/current =====
V = smpl(:, 1:8);
I = smpl(:, 9:16);

%% ===== Detect PGMTime decreases =====
tol = 1e-6;  % tolerance for float jitter
dpgm = diff(pgm);
decIdx = find(dpgm < -tol) + 1;
resetTimes = time(decIdx);

%% ===== Plot results =====
figure('Color','w','Name',sprintf('Waveforms: %s', fn));
tiledlayout(2,1);

% ---- Voltages ----
nexttile;
plot(time, V, 'LineWidth', 1.0);
grid on; xlabel('TIME (hours)'); ylabel('Voltage (V)');
title('Voltages (Channels 1–8)'); hold on;
for k = 1:numel(resetTimes)
    xline(resetTimes(k),':k', 'LineWidth',1); 
end
if showLegends
    legend(arrayfun(@(k)sprintf('V%d',k),1:8,'UniformOutput',false),'Location','eastoutside');
end

% ---- Currents ----
nexttile;
plot(time, I, 'LineWidth', 1.0);
grid on; xlabel('TIME (hours)'); ylabel('Current (mA)');
title('Currents (Channels 1–8)'); hold on;
for k = 1:numel(resetTimes)
    xline(resetTimes(k),':k', 'LineWidth',1); 
end
if showLegends
    legend(arrayfun(@(k)sprintf('I%d',k),1:8,'UniformOutput',false),'Location','eastoutside');
end

linkaxes(findall(gcf,'Type','axes'),'x');
