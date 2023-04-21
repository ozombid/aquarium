import java.net.*;
import java.io.*;


public class Client {

    private static final int BUFFER_SIZE = 255;
    
    public static final void error(String str) {
        System.err.println(str);
        System.exit(1);
    }

    public static String readFromServer(Socket socket) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        StringBuilder sb = new StringBuilder();
        int c;
        while ((c = in.read()) != -1) {
            sb.append((char) c);
            if (in.ready()) continue;
            break;
        }
        return sb.toString();
    }

    public static void main(String[] args) 
    {
        if (args.length < 2) error("ERROR: no port/hostname provided");

        /* 1 -- initialization */
            // set port
            int portno = Integer.parseInt(args[1]);
            // initialize socket
            Socket socket = null;
            try {
                socket = new Socket(args[0], portno);
            } catch (UnknownHostException e) {
                error("ERROR: unknown host");
            } catch (IOException e) {
                error("ERROR: could not open socket");
            }
            // set input and output streams
            PrintWriter out = null;
            BufferedReader in = null;
            BufferedReader stdIn = null;
            try {
                out = new PrintWriter(socket.getOutputStream(), true);
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                stdIn = new BufferedReader(new InputStreamReader(System.in));
            } catch (IOException e) {
                error("ERROR: could not get input/output streams");
            }

        /* 2 -- communication */
            String inputLine, outputLine;
            try {
                while (true) {
                    // read
                    inputLine = readFromServer(socket);
                    System.out.print("> " + inputLine);
                    // write 
                    outputLine = stdIn.readLine(); // adds a newline at end 
                    if (outputLine == null) error("ERROR: null message sent");
                    if (outputLine.equals("bye")) break;
                    out.println(outputLine);
                    if (outputLine.equals("down")) break;
                }
            } catch (IOException e) {
                error("ERROR: communication error");
            }

        /* 3 -- close */
        try {
            out.close();
            in.close();
            stdIn.close();
            socket.close();
        } catch (IOException e) {
            error("ERROR: could not close socket");
        }
    }
}


/*
int c;
StringBuilder sb = new StringBuilder();
while (in.ready() && (c = in.read()) != -1 ) sb.append((char) c);
inputLine = sb.toString();
if (inputLine == null) error("ERROR: null message received");
*/
