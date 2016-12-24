import java.io.File;
import java.io.FileInputStream;
import java.io.PrintWriter;
/**
 * Transform HTML file to byte array inside C++ library file 
 */
class GenerateWebSwitchH {
  public static void main(String args[]) throws Exception {
	 PrintWriter pw = new PrintWriter(new File("WebSwitch.h"));
	 pw.println("#ifndef WebSwitch_H");
	 pw.println("#define WebSwitch_H");
	 
	 pw.println("const char webpage[]={");
	 FileInputStream fr = new FileInputStream("WebSwitch.html");
	 byte temp[] = new byte[100];
	 int readed = 0;
	 while((readed = fr.read(temp))>0) {
		for(int i=0;i<readed;i++) {
		  pw.print("0x"+Integer.toHexString(temp[i])+","); 
		}
		pw.println();
	 }
	 fr.close();
	 pw.println("0x00};");//Zero ended string
	 
	 pw.println("#endif");
	 pw.close();
  }
}