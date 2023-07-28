for i in {0..40}
do

    for j in {0..20}
	do
        sleep 0.1
        offset=$(echo "scale=2; -2.0 + 0.1 * ${i}" | bc)
        rotation=$(echo "scale=2; 4.0 + 0.1*${j}" | bc)
        
        echo $offset $rotation
        nohup root -l -b -q check_correlation.C\(\{${offset},${rotation}\}\)&>out.out&
    done  
    echo
    sleep 10
done