package main

import (
	"bufio"
	"fmt"
	"os"
	"time"
)

func main() {
	var myId int
	mesInit := MessageCall(MR_BROKER, MT_INIT, "")
	myId = int(mesInit.Header.To)
	go func() {
		for {
			m := MessageCall(MR_BROKER, MT_GETDATA, "")
			if m.Header.Type == MT_DATA {
				fmt.Println(m.Data)
			} else if m.Header.Type == MT_CLIENTS_LIST {
				fmt.Println("My Id: ", myId)
				fmt.Println("Available users: ", m.Data, "50 - all users")
			} else {
				time.Sleep(time.Second * 1)
			}
		}
	}()
	time.Sleep(time.Second)
	reader := bufio.NewReader(os.Stdin)
	for {
		var addr int32
		fmt.Println("Enter recipient: ", "(-1 to disconnect)")
		fmt.Scanln(&addr)

		if addr == -1 {
			MessageCall(MR_BROKER, MT_EXIT, "")
			return
		}

		fmt.Println("Enter message: ")
		s, err := reader.ReadString('\n')
		if err != nil {
			continue
		}
		message := fmt.Sprintf("[%d] ", myId) + s
		MessageCall(addr, MT_DATA, message)
	}
}
