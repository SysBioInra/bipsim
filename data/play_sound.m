function play_sound ()
%% To play a sound
    T=419*2; T2=419*4; t=[1:.3:T]; t2=[1:.3:T2];
    S=(sin(t*pi/T).^.2+sin(t*pi/T).^2).*sin((t+sin(t/2)*.2).^1.0)/2;
    S2=(sin(t2*pi/T2).^.4+sin(t2*pi/T2).^4).*sin((t2+sin(t2/2)*.2)*1.5)/2;
    sound([S S],8192); sound(S2,8192);
end