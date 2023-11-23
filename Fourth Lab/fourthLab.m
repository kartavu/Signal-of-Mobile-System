LENGTH = 5;

cons = 2^LENGTH - 1;
register_state_x = [0, 1, 0, 1, 0];
register_state_y = [1, 0, 0, 0, 1];
    
register_state_x1 = [0, 1, 0, 1, 1];
register_state_y1 = [0, 1, 1, 0, 0];

pseudo_random_sequence = generate_pseudo_random_sequence(register_state_x, register_state_y, cons);
modified_sequence = second_generate_pseudo_random_sequence(register_state_x, register_state_y, cons);

lags = 0:cons-1;
autocorr_values = xcorr(pseudo_random_sequence - mean(pseudo_random_sequence), cons-1, 'coeff');
fprintf('Сдвиг|1 |2 |3 |4 |5 |6 |7 |8 |9 |10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|Автокорреляция\n');
%disp(autocorr_values)
for shift = 0:cons-1
        fprintf('%5d|', shift);
        shifted_sequence = circshift(pseudo_random_sequence, [0, shift]);
        fprintf('%2d|', shifted_sequence);
        
        autocorrelation = calculate_autocorrelation(pseudo_random_sequence, shifted_sequence);
        fprintf('%.0f/31', autocorrelation);
        fprintf('\n');
 end

figure;
plot(lags, autocorr_values(cons:end));
title('Autocorrelation of Pseudo-Random Sequence');
xlabel('Лаг');
ylabel('Автокорреляция');

crosscorr_values = xcorr(pseudo_random_sequence - mean(pseudo_random_sequence), modified_sequence - mean(modified_sequence), 'coeff');

figure;
plot(-cons+1:cons-1, crosscorr_values);
title('Crosscorrelation of Pseudo-Random and Modified Sequences');
xlabel('Лаг');
ylabel('Взаимная корреляция');


function register_state_x = shift_register_x(register_state_x)
    feedback = mod(register_state_x(3) + register_state_x(4), 2);
    register_state_x(2:end) = register_state_x(1:end-1);
    register_state_x(1) = feedback;
end

function register_state_y = shift_register_y(register_state_y)
    feedback = mod(register_state_y(2) + register_state_y(3), 2);
    register_state_y(2:end) = register_state_y(1:end-1);
    register_state_y(1) = feedback;
end

function sequence = generate_pseudo_random_sequence(register_state_x, register_state_y, length)
    fprintf('\n');
    fprintf('Последовательность Голда равняется: ');
    sequence = zeros(1, length);
    
    for i = 1:length
        sequence(i) = mod(register_state_x(5) + register_state_y(5), 2);
        fprintf('%d', sequence(i));
        register_state_x = shift_register_x(register_state_x);
        register_state_y = shift_register_y(register_state_y);
    end
    
    fprintf('\n\n');
end

function sequence = second_generate_pseudo_random_sequence(register_state_x, register_state_y, length)
    fprintf('Измененная последовательность Голда равняется: ');
    sequence = zeros(1, length);
    
    for i = 1:length
        sequence(i) = mod(register_state_x(5) + register_state_y(5), 2);
        fprintf('%d', sequence(i));
        register_state_x = shift_register_x(register_state_x);
        register_state_y = shift_register_y(register_state_y);
    end
    
    fprintf('\n');
end

function shifted_sequence = cyclic_shift(sequence)
    temp = sequence(end);
    shifted_sequence = [temp, sequence(1:end-1)];
end

function autocorr_value = calculate_autocorrelation(sequence1, sequence2)
    matches = sum(sequence1 == sequence2) - sum(sequence1 ~= sequence2);
    autocorr_value = matches;% / length(sequence1);
end

function crosscorr_value = calculate_crosscorrelation(sequence1, sequence2)
    crosscorr_value = sum(sequence1 .* sequence2);
end
