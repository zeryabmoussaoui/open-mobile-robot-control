function [ output ] = compute_controler(response , input , sample_time , desired_tau )

% TODO : add documentation
plot(response)
N = size(response);
K = response(N(2))/ input ; %static gain
indice = find(response > response(N(2))*0.85 , 1) ; % take the third value of 90% of static gain
tau = indice*sample_time/1000 % constante time in seconde
Ki = 1/(K*desired_tau) ;
Kp = Ki*tau ;
output = [Kp  Ki ]; 
end

