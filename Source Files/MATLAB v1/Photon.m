classdef Photon < handle
    %Photon defines a class for Particle photon objects.
    %   Instantiate Particle photon objects as obj = Photon(name,
    %   accestoken), where name is the name or coreID of the Photon and
    %   the accesstoken is the access token given by the website
    %   Requires cURL to flash local code the Photons.
    properties
        coreID;
        url;
        url2;
    end
    
    properties (SetAccess = protected, GetAccess = private)
        token
    end
    
    methods
        %Builder: Each Photon has a name and a token
        function obj = Photon(name, accestoken)
            obj.coreID = name;
            obj.token = accestoken;
            obj.url = strcat('https://api.particle.io/v1/devices/',obj.coreID,'/');
            obj.url2 = strcat('https://api.particle.io/v1/devices/');
        end
        %fetch takes a single parameter, the name of the variable you want
        %to fetch from the spark.io website.  Returns the value of that
        %parameter. Pass each argument as a string!
        function data  = fetch(obj,param)
            URL = strcat(obj.url,param,'/');
            data = webread(URL,'access_token',obj.token);
            data = data.result;
        end
        
        function names  = getDevices(obj)
            URL = obj.url2;
            data = webread(URL,'access_token',obj.token);
              if iscell(data)%We are dealing with a Spark Core/Photon Combo
                    for j = 1:length(data)
                        %Spark and Photon return different size structs
                        %when queried with webread, just get data we use
                        dum(j).name = getfield(data{j},'name');  
                    end
                    clear data
                    data = dum;
              end
            names = {};
            for i = 1:length(data)
                names{i} = data(i).name;
            end
        end
        
        function connected  = getConnection(obj)
            URL = obj.url2;
            data = webread(URL,'access_token',obj.token);
            whos
              if iscell(data)%We are dealing with a Spark Core/Photon Combo
                    for j = 1:length(data)
                        %Spark and Photon return different size structs
                        %when queried with webread, just get data we use
                        dum(j).name = getfield(data{j},'name');  
                        dum(j).connected = getfield(data{j},'connected');
                        dum(j).id = getfield(data{j},'id');
                    end
                    clear data
                    data = dum;
              end
            names = {};
            connection={};
            connected = 0;
            for i = 1:length(data)
                names{i} = data(i).name;
                connection{i} = data(i).connected;
                if strcmp(names{i},obj.coreID) || strcmp(data(i).id,obj.coreID)
                    connected = connection{i}(1);
                end
            end
        end
        
        function names  = getConnectedDevices(obj)
            URL = obj.url2;
            data = webread(URL,'access_token',obj.token);
            dum = struct([]);
                if iscell(data)%We are dealing with a Spark Core/Photon Combo
                    for j = 1:length(data)
                        %Spark and Photon return different size structs
                        %when queried with webread, just get data we use
                        dum(j).name = getfield(data{j},'name');  
                        dum(j).connected = getfield(data{j},'connected');
                    end
                    clear data
                    data = dum;
                end
            names = {};
            m = 1;
            for i = 1:length(data)
                if data(i).connected(1)
                names{m} = data(i).name;
                m = m+1;
                end
            end
        end
        
        %getVariables returns the potential variables that can be fetched
        function vars  = getVariables(obj)
            URL = strcat(obj.url);
            a = webread(URL,'access_token',obj.token);
            vars = a.variables;
        end
        
        %getFunctions returns the potential functions that can be pushed
        function funcs  = getFunctions(obj)
            URL = strcat(obj.url);
            a = webread(URL,'access_token',obj.token);
            funcs = a.functions;
        end
        
        %push takes a single parameter, the name of the variable you want
        %to fetch from the spark.io website.  Returns the value of that
        %parameter. Pass each argument as a string!
        function feedback  = push(obj,func,value)
            options = weboptions('Timeout',20);
            URL = strcat(obj.url,func,'/');
            a = webwrite(URL,'access_token',obj.token,'args',value,options);
            feedback = a.return_value;
        end
        
        %flash code contained locally to the particle photon
        function feedback  = flash(obj,file)
            URL = strcat('curl -X PUT -F file=@',file);
            URL = strjoin({URL,obj.url})
            URL = strjoin({URL,' -H ''Authorization: Bearer',obj.token})
            URL = strcat(URL,'''')
            data = unix(URL)
        end
        %move servo to angle
        function feedback = move(obj,angle)
            feedback = obj.push('move',angle);
        end
        
        %Attach a servo to a pin given as a string
        function feedback = attachServo(obj,pin)
            feedback = obj.push('attachServo',pin);
        end
        
        %Detach a servo to a pin given as a string
        function feedback = detachServo(obj)
            feedback = obj.push('detachServo','');
        end
        
        %setInput pin
        function feedback = setInput(obj,pin)
            feedback = obj.push('setInput',pin);

        end
        
        %setOuput pin
        function feedback = setOutput(obj,pin)
            feedback = obj.push('setOutput',pin);
        end
        
        %setInput pin
        function feedback = getPinMode(obj,pin)
            feedback = obj.push('pinMode',pin);
            if feedback ==1;
                fprintf('INPUT\n')
            else
                fprintf('OUTPUT\n')
            end
        end
        
        %reads the value of an analog pin returning a value of 0-3.33V
        function feedback = analogRead(obj,pin)
            %{
            str = obj.fetch('String');
            str = strsplit(str,',');
            obj.getPin(pin)+1;
            feedback = str2num(str{obj.getPin(pin)+1});
            %}
            obj.setInput(pin);
            feedback = obj.push('analogRead',pin); %Read in as bit value
            feedback = feedback .* 3.33 ./ 2^12; %Convert from 12 bit value
        end
        
        %reads the value of a digital pin
        function feedback = digitalRead(obj,pin)
            obj.setInput(pin);
            feedback = obj.push('digitalRead',pin);
        end
        
        %writes an analog voltage value to an analog pin
        function feedback = analogWrite(obj,pin,value)
            if value<0.0 || value>3.33
                warning('Analog voltage set outside range (0 to 3.33 V)!')
            else
            value = round(value .* 255./3.33); %Convert to 8 bit value
            obj.setOutput(pin);
            feedback = obj.push('analogWrite',strcat(pin,',',int2str(value)));
            end
        end
        
        %writes a 0 or 1 value to a digital pin
        function feedback = digitalWrite(obj,pin,value)
            obj.setOutput(pin);
            feedback = obj.push('digitalWrite',strcat(pin,',',int2str(value)));
        end
        
        %Set the frequency of the analogWrite
        function feedback = setFreq(obj,value)
            feedback = obj.push('setFreq',value);
        end
        
        %Return the frequency of a pwm pin input  
        function feedback = getTone(obj,pin)
            feedback = obj.push('getPulse',pin);
        end
        
        %Return whether a pin is an input or an output 
        function feedback = getPin(obj,pin)
            feedback = obj.push('getPin',pin);
        end
        
        %Read Pressure from Cylinder object
        function [pt, pb] = mrcylinder(obj)
            options = weboptions('Timeout',20);
            %%% GET DATA %%%
            func = 'Pressure';
            URL = strcat(obj.url,func,'/');
            a = webread(URL,'access_token',obj.token,options);
            pn = str2double(strsplit(a.result,',')); 
            pn = pn ./ 2^11; %Convert to fractions of range
           %%% Conversion Factors %%%
            %First 8 taps top and bottom from 0 are 2.5kPa
            %Last 3 taps are 1kPa, see notes on github for tap layout
            %Tap numbering wraps around cylinder
            %Rear-most transducers top and bottom not connected!!
            pt = pn([4,3,2,1,8,7,6,5,12,11,10])'; %Map to location
            pb = pn([15,14,13,20,19,18,17,24,23,22])';
            pt = [pt(1:8).*2500; pt(9:11).*1000];% Calibrate %
            pb = [pb(1:3).*1000; pb(4:end).*2500];

        end
        
        %Read Pressure from Airfoil object
        function [pt,pb,aoa] = mrsairfoil(obj)
            %Zeros:%
            options = weboptions('Timeout',20);
            func = 'Angle';
            URL = strcat(obj.url,func,'/');
            temp = webread(URL,'access_token',obj.token,options);
            aoa = temp.result; %Get angle of attack value
            %%% GET Pressure DATA %%%
            func = 'Pressure';
            URL = strcat(obj.url,func,'/');
            a = webread(URL,'access_token',obj.token,options);
            pn = str2double(strsplit(a.result,',')); 
            pn = pn ./ 2^11; %Convert to fractions of range
           %%% Conversion Factors %%%
            %MUX TOP: CH 0-5 are 1PSI, rest 1 kPa
            %MUX Bottom: CH 0-15 are all 1kPa
            %%% Only 1-29 and 31 are connected to ptaps %%%
            %%% CH 8 (index from 1 @ L.E.) is bad, replace %%%
            p = [pn(1:6).*6894.757 pn(7:end).*1000]; %Convert to Pascals
            pt = p(1:16)';
            pt(8) = (pt(7) + pt(9))/2; %Tap #8 went south for some reason
            pb = p([17:29,31])';
        end
        
        
        %%EXPERIMENTAL
        function pinOuts(obj)
            left = '<--';
            right = '-->';
            sp5 = '     ';
            sp4 = '    ';
            i = 'INPUT';
            o = 'OUTPUT';
            s = 'SERVO';
            string = obj.fetch('String2');
            string = strsplit(string,',');
            temp = sprintf('\n\t\t\t ___________ \t\t\t');
            temp = strcat(temp,'\n');
            for q = 1:8
                if str2num(string{q+10})==1 
                    temp = strcat(temp,'\t',i,'\t',right,'\t','|O:A',num2str(q-1),' \t');
                elseif str2num(string{q+10})==0
                    temp = strcat(temp,'\t',o,'\t',left,'\t','|O:A',num2str(q-1),' \t');
                elseif str2num(string{q+10})==2
                    temp = strcat(temp,'\t',s,'\t',left,'\t','|O:A',num2str(q-1),' \t');
                else
                    temp =strcat(temp,'\t\t\t','|O:A',num2str(q-1),' \t');
                end
                if str2num(string{q})==1 
                    temp = strcat(temp,'D',num2str(q-1),':O|  \b',right,'   \b',i,'\n');
                elseif str2num(string{q})==0
                    temp = strcat(temp,'D',num2str(q-1),':O|  \b',left,'  \b',o,'\n');
                elseif str2num(string{q})==2
                    temp = strcat(temp,'D',num2str(q-1),':O|  \b',right,'  \b',s,'\n');               
                else
                    temp = strcat(temp,'D',num2str(q-1),':O|','\t\t\t','\n');
                end
            end
            %temp = strcat(temp,'\t\t\t','|\t\t\t|','\t\t\t','\n');
            temp = strcat(temp,'\t\t\t',' \\\t   /','\t\t\t','\n');
            temp = strcat(temp,'\t\t\t','  \\_______/','\t\t\t','\n\n');
            fprintf(temp)
        end
    end
end

