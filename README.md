# UDP Client Program with Go-Back-N Protocol

This C program implements a UDP client using the Go-Back-N protocol. The client sends a user-input string to a server over a UDP connection and utilizes the Go-Back-N mechanism for reliable data transfer. The Go-Back-N protocol allows the client to send multiple segments before receiving acknowledgments, and it handles both lost and out-of-order acknowledgments.

## Usage

- **Compile:** `gcc client.c -o client`
- **Run:** `./client <server_ip> <server_port>`

## Program Flow

1. **User Input:**
   - The program prompts the user to enter a string.

2. **Sending Length of String:**
   - The program calculates the length of the user-entered string and sends the length to the server.

3. **Segmenting and Sending Data:**
   - The program segments the string into two characters per segment (except the last one, if the length is odd).
   - For each segment, the program creates a buffer containing the sequence number, segment length, and the two characters.
   - The program sends multiple segments to the server without waiting for acknowledgments.

4. **Acknowledgment Handling:**
   - The program receives acknowledgments from the server.
   - It checks the acknowledgment sequence number and updates the window accordingly.
   - The program continues sending new segments and handling acknowledgments until all segments are acknowledged.

5. **Closure:**
   - Once all segments have been acknowledged, the program closes the UDP socket.

## Go-Back-N Protocol Features

- **Sliding Window:**
  - The program maintains a sliding window for tracking sent and acknowledged segments.

- **Multiple Segment Transmission:**
  - The client sends multiple segments before waiting for acknowledgments, improving efficiency.

- **Acknowledgment Timeout:**
  - Timeouts are used to trigger retransmissions for segments that were not acknowledged within a specified timeframe.

## Important Notes

- The program uses UDP for communication, and the Go-Back-N protocol is implemented for reliable data transfer.
- The acknowledgment contains the sequence number of the successfully received segment.
