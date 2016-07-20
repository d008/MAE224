classdef Photon < handle
    %Photon defines a class for Particle photon objects.
    %   Instantiate Particle photon objects as obj = Photon(name,
    %   accestoken), where name is the name or coreID of the Photon and
    %   the accesstoken is the access token given by the website
    
    properties
        coreID;
        url;
    end
    
    properties (SetAccess = protected, GetAccess = private)
        token
    end
    methods
        %Builder: Each SparkCore has a name and a token
        function obj = Photon(name, accestoken)
            obj.coreID = name;
            obj.token = accestoken;
            obj.url = strcat('https://api.particle.io/v1/devices/',obj.coreID,'/');
        end
        %fetch takes a single parameter, the name of the variable you want
        %to fetch from the spark.io website.  Returns the value of that
        %parameter. Pass each argument as a string!
        function data  = fetch(obj,param)
            URL = strcat(obj.url,param,'/');
            data = webread(URL,'access_token=',obj.token);
            data = data.result;
        end
        
        %getVariables returns the potential variables that can be fetched
        function vars  = getVariables(obj)
            URL = strcat(obj.url);
            a = webread(URL,'access_token=',obj.token);
            vars = a.variables;
        end
        
        %getFunctions returns the potential functions that can be pushed
        function funcs  = getFunctions(obj)
            URL = strcat(obj.url);
            a = webread(URL,'access_token=',obj.token);
            funcs = a.functions;
        end
        
        %push takes a single parameter, the name of the variable you want
        %to fetch from the spark.io website.  Returns the value of that
        %parameter. Pass each argument as a string!
        function feedback  = push(obj,func,value)
            URL = strcat(obj.url,func,'/');
            a = webwrite(URL,'access_token',obj.token,'args',value);
            feedback = a.return_value;
        end
        
        function feedback  = flash(obj,file)
            URL = strcat('curl -X PUT -F file=@',file);
            URL = strjoin({URL,obj.url})
            URL = strjoin({URL,' -H ''Authorization: Bearer',obj.token})
            URL = strcat(URL,'''')
            data = unix(URL)
        end
        
    end
end

