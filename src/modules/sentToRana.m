function output = sentToRana(t, str)

    fprintf(t, '%s', str);
    output = convertCharsToStrings(fscanf(t));
    %output = convertCharsToStrings(fscanf(t))
    
end