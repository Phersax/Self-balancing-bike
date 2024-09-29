clc
close all
 
N = length(x);

% valutazione coeff . dell ’ approssimante ai minimi quadrati 
p = polyfit(x, y, 1);
slope = p(1);
offset = p(2);

% valore ”p 1” nelle ascisse ”x”
z = polyval(p, x);

% errore ||f−p 1|| 2
err2 = norm( z - y, 2);

%stima dell'errore in uscita:
dev_st_2 = ((sum(y - offset - slope*x))^2)/(N - 2);

fprintf('Errore regressione norma2: %1.2e \n', err2);

% plot punti
plot(x, y, 'go', 'LineWidth' , 1, ... 
    'MarkerEdgeColor', 'k', ...
    'MarkerFaceColor', 'g' ,...
    'MarkerSize', 10);
hold on;

% plot retta regressione
plot(x, z); 

% titoli e legenda
title('Regressione lineare');
legend('Dati', 'Retta di regressione')
legend();
xlabel('Pwm');
ylabel('RPM');
hold off;