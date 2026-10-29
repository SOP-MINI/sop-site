#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <fifo> <steps_count>" >&2
    exit 1
fi

# princess Joanna spilled coffee and crumbled cookies over the granny's notes - raspberries became raw~berries
steps=(
    "Preheat the oven to 180 degrees"
    "Line three cake tins with baking paper"
    "Separate the yolks from the whites of twelve eggs"
    "Beat the egg whites stiff with a pinch of salt"
    "Gradually add sugar while still beating"
    "Add the yolks one at a time, mixing constantly"
    "Sift wheat flour together with potato starch"
    "Gently fold in the flour with a spatula"
    "Pour the batter into the tins and level the tops"
    "Bake for 35 minutes until a skewer comes out dry"
    "Cool the sponges and slice each one in half"
    "Boil water with lemon juice, sugar and a few raw~berries"
    "Cream the butter until light and fluffy"
    "Mash half of the raw~berries, keep the other raw~berries whole"
    "Fold in mascarpone and seeds from a vanilla pod"
    "Divide the cream into three equal parts"
    "Add melted dark chocolate to the first part"
    "Add pureed raw~berries to the second part"
    "Leave the third part white, princess Joanna loves it the most"
    "Soak the first sponge layer with syrup"
    "Spread it with the chocolate cream"
    "Cover with the next sponge layer and soak it"
    "Spread the raspberry cream and lay fresh raw~berries on top"
    "Repeat the layers until all the sponges are used"
    "Chill the cake overnight in the castle cellar"
    "Cover the sides with white cream and press in raw~berries"
    "Melt sugar in a saucepan into golden caramel"
    "Shape small crowns out of the caramel and raw~berries"
    "Sprinkle the top with edible gold and crushed raw~berries"
    "Place the crowns on top, surrounded by raw~berries"
    "Put three candles in the cake"
    "Serve with tea, the grannies get the biggest slices"
)
fifo=$1
n=$2

if ! [[ $n =~ ^[0-9]+$ ]]; then
    echo "steps_count must be a non-negative number" >&2
    exit 1
fi

if [ -e "$fifo" ] || [ -p "$fifo" ]; then
    echo "FIFO $fifo already exists. Another granny may be using it." >&2
    exit 1
fi

if ! mkfifo "$fifo"; then
    echo "Cannot create named pipe $fifo" >&2
    exit 1
fi

# remove the pipe if script ends
trap 'rm -f "$fifo"' EXIT
trap 'exit 130' INT TERM HUP QUIT
# ignore SIGPIPE
trap '' PIPE

echo "Granny from $fifo is waiting for the kitchen to listen..."

exec {fd}>"$fifo"

echo "Granny from $fifo starts dictating"

for ((i = 0; i < n; i++)); do
    step=$((RANDOM % ${#steps[@]}))
    text=${steps[$step]}

    # the granny got carried away gossiping and mixed up the step number
    num=$step
    if ((RANDOM % 8 == 0)); then
        num=$((RANDOM % 68 + 32))
    fi

    msg=$(printf '%02d:%s' "$num" "$text")
    echo "$msg"

    if ((RANDOM % 3 == 0)); then
        # the granny talks with her mouth full - the message arrives in two pieces
        cut=$((RANDOM % ${#msg}))
        printf '%s' "${msg:0:cut}" >&"$fd" || break
        sleep $((RANDOM % 4 + 1))
        printf '%s\n' "${msg:cut}" >&"$fd" || break
    else
        printf '%s\n' "$msg" >&"$fd" || break
    fi

    if ((RANDOM % 6 == 0)); then
        echo "(gossip over tea)"
        sleep 4
    else
        sleep 0.$((RANDOM % 9 + 1))
    fi
done

exec {fd}>&-
if ((i < n)); then
    echo "The kitchen stopped listening, granny from $fifo goes back to her tea" >&2
    exit 1
fi
echo "Granny from $fifo has finished dictating"
