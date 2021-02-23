
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import javax.imageio.ImageIO;
import java.nio.charset.StandardCharsets;
  
public class UDPServer { 

    private BufferedImage img;
    private ByteArrayOutputStream bout;
    private byte[] buf_img;
    private int len,start,end;
    private int i;
    private String msg_send = "Picture length in bytes:";
    private byte[] buf_send,buf_slice,buf_rec,buf_slice_str;
    private DatagramPacket packet;
    private int port_clnt;
    private InetAddress ip_clnt;
    private final DatagramSocket socket;
    private StringBuilder stringbuild;
    private String str;
    
    public UDPServer(int port) throws IOException {
        
        socket = new DatagramSocket(port);
        port_clnt = 4455;
        ip_clnt = InetAddress.getLocalHost();
        
        img = ImageIO.read(new File("testfile.jpg"));
        bout = new ByteArrayOutputStream();
        ImageIO.write(img, "jpg", bout);
        buf_img = bout.toByteArray();
        
        len = buf_img.length;
  
        msg_send = msg_send + String.valueOf(len);         

        buf_send = msg_send.getBytes();
        packet = new DatagramPacket(buf_send, buf_send.length, ip_clnt, port_clnt);
        socket.send(packet);
        
        start = 0;
        end = 62975; //KB
                
        while(true) {
            
            buf_slice = getSliceOfArray(buf_img,start,end);
            
            packet = new DatagramPacket(buf_slice,buf_slice.length,ip_clnt,port_clnt);
            socket.send(packet);
            
            while(true) {
                
                buf_rec = new byte[1024];
                
                packet = new DatagramPacket(buf_rec,buf_rec.length);
                socket.receive(packet);
                stringbuild = data(buf_rec);
                
                System.out.println("Client:" + stringbuild);
                
                if(stringbuild.toString().equals("NAK")) {
                    
                    packet = new DatagramPacket(buf_slice,buf_slice.length,ip_clnt,port_clnt);
                    socket.send(packet);
                }
                else {
                    
                    break;
                }
            }
            
            start = end + 1;
            
            if(start > len - 1) {
               
               break;
            }
            if(start + 62975 > len - 1) {
                
               end = len - 1;
            }
            else {
                
                end = start + 62975;
            }
        }
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
    
    public byte[] getSliceOfArray(byte[] arr, int start, int end) {
        
        byte[] slice = new byte[end - start + 1];
        
        for (int i = 0; i < slice.length; i++) { 
            slice[i] = arr[start + i]; 
        }
        
        return(slice);
    }
    
    public static void main(String[] args) throws IOException { 
         
        UDPServer udp_srv = new UDPServer(17);
    } 
  
    
} 
