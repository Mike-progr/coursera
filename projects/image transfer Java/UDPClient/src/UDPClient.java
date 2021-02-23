
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import javax.imageio.ImageIO;


public class UDPClient { 

    private DatagramSocket socket;
    private ByteArrayInputStream bis;
    private byte[] buf_rec,buf_send,buf;
    private DatagramPacket packet;
    private int size,packet_num,array_pntr;
    private StringBuilder stringbuild;
    private BufferedImage img;
    private String msg;
    private InetAddress ip_srv;
    
    public UDPClient(int port) throws SocketException, IOException {
        
        socket = new DatagramSocket(port);
        ip_srv = InetAddress.getLocalHost();
        
        buf_rec = new byte[65535];
            
        packet = new DatagramPacket(buf_rec,buf_rec.length);
        socket.receive(packet);
        stringbuild = data(buf_rec);
        System.out.println("Server:"  + stringbuild);
       
        size = Integer.valueOf(stringbuild.toString().substring(stringbuild.toString().lastIndexOf(':') + 1));
        
        buf_rec = new byte[size];
        
        packet_num = 0;
        array_pntr = 0;
        
        while(true) {
            
            buf = new byte[62976];
            msg = "NAK";
            
            do{
                try {
                
                    packet = new DatagramPacket(buf,buf.length);
                    socket.receive(packet);
               
                    msg = "ACK";
               
                    buf_send = msg.getBytes();
                    packet = new DatagramPacket(buf_send, buf_send.length, ip_srv, 17);
                    socket.send(packet);
               
                }catch(SocketTimeoutException exception) {
                
                    msg = "NAK";
               
                    buf_send = msg.getBytes();
                    packet = new DatagramPacket(buf_send, buf_send.length, ip_srv, 17);
                    socket.send(packet);
                }
            }while(msg.equals("NAK"));
            
            packet_num++;
            
            System.out.println(packet_num);
            
            if(array_concat()){
             
                break;
            }

        }
        
        bis = new ByteArrayInputStream(buf_rec);
        img = ImageIO.read(bis);
        ImageIO.write(img, "jpg", new File("output.jpg") );
        
        socket.close();
    }
    
    public boolean array_concat() {
        
        int i;
        boolean ret_val;
        
        ret_val = false;
        
        for(i=0; i<62976; i++) {
            
            buf_rec[array_pntr] = buf[i];
            array_pntr++;
           
            if(array_pntr == size) {
                
                ret_val = true;
                break;
            }
        }
      
        return(ret_val);
    }
    
    public static StringBuilder data(byte[] a) { 
        
        if (a == null) 
            return null; 
        
        StringBuilder ret = new StringBuilder(); 
        int i = 0; 
        
        while (a[i] != 0) { 
        
            ret.append((char) a[i]); 
            i++; 
        }
        
        return ret; 
    }
    
    public static void main(String args[]) throws SocketException, IOException { 
        
        UDPClient clnt = new UDPClient(4455);
    } 
} 