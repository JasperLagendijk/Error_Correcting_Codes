%% Type of parity matrix
clear all, close all
CodeRate = 1/2; % can be 1/2, 2/3, 3/4 or 5/6
Ncoded = 648;  % can be 648, 1296, 1944

%% Create Matrix
Hldpc      = WL11_LDPCmatrix(CodeRate,Ncoded);
%Hldpc = [   1 0 0 1 1 1 0 0;
%                0 1 1 1 0 0 1 1;
%                1 1 1 0 1 0 0 1;
%                 0 0 1 0 1 1 1 1];


Hsprs      = logical(sparse(Hldpc));
encodercfg = ldpcEncoderConfig(Hsprs);
Data =zeros(Ncoded*CodeRate, 1);
Data(1) = 1;
Coded = ldpcEncode(Data, encodercfg);
%Coded(325:352)
%% Save Sparse parity matrix
save("parity.mat", "Hldpc");