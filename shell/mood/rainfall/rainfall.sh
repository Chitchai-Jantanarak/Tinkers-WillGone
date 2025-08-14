echo -ne "\e[?25l"

declare -a x y speed symbol color

init_drop() {
    ROWS=$(tput lines)
    COLS=$(tput cols)
    NUM_DROPS=$((COLS / 2))

    for ((i=0; i<NUM_DROPS; i++)); do
        x[i]=$(( RANDOM % COLS ))
        y[i]=$(( RANDOM % ROWS ))
        speed[i]=$(( 1 + RANDOM % 3 ))
        symbol[i]="|.|"
        case $((RANDOM % 3)) in
            0) color[i]=34 ;;
            1) color[i]=36 ;; 
            2) color[i]=94 ;;
        esac
    done
}

handle_resize() {
    init_drop
}

cleanup() {
    echo -ne "\e[?25h"
    clear
    exit
}

trap handle_resize SIGWINCH     # Resizing
trap cleanup INT TERM EXIT      # ^C Terminate

init_drop

while [[ true ]]; do
    echo -ne "\e[H"
    clear
    for ((i=0; i<NUM_DROPS; i++)); do
        row=${y[i]}
        col=${x[i]}
        char=${symbol[i]:$((RANDOM % ${#symbol[i]})):1}
        echo -ne "\e[$((row+1));$((col+1))H\e[${color[i]}m$char\e[0m"

        y[i]=$(( (y[i] + speed[i]) % ROWS))
    done

    sleep 0.05 
done

