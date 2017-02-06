% INPUTS %
photon = 'your_photon_name'; %Put your specific Photon name here
atoken = 'abc123';           %Access token
apin = 'A4';    %Analog pin for PWM output, note that only specific Photon pins are PWM capable
readpin = 'A5'; %Pin to read in analog frequency
vout = 3;       %PWM Voltage out
freq = 3000;    %Frequency for analog PWM


%% MAIN PROGRAM %%
%Create a new photon object%
g = Photon(photon,atoken);
%Display the connected devices
g.getConnectedDevices()

if g.getConnection %Only run code if Photon is connected
    g.setFreq(freq) %Set the PWM writing frequency
    g.analogWrite(apin,vout); %Output an analog voltage

    f1v = g.getTone(readpin); %Read frequency at pin A5
    disp(['Frequency at pin ' readpin ' is ' num2str(f1v) ' Hz. '])

end
