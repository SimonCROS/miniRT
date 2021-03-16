cd ..
for entry in "scenes/crash"/*.rt
do
	echo "\033[1;31mTesting $entry...\033[0m"
	./miniRT $entry &> /dev/null &
	TASK_PID=$!
	sleep 0.5
	leaks miniRT | sed '1,/leaks Report/d'
	kill $TASK_PID
	wait $TASK_PID &> /dev/null
done
