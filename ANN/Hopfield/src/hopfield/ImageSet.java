/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package hopfield;

import java.awt.image.BufferedImage;
import java.awt.image.DataBuffer;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;

/**
 *
 * @author yova
 */
public class ImageSet {
    public Map<Integer,List<Integer>> imageMap =  new HashMap<Integer,List<Integer>>();
    public Integer imageWidth;
    public Integer imageHeight;
    
    public ImageSet(int width, int height){
        this.imageHeight = height;
        this.imageWidth = width;
    }
    
    public void addImage(Integer key, String fileName){
        File imageFile = new File( fileName );
        BufferedImage image;
        Integer pixel;
        List<Integer> pixelList = new ArrayList<Integer>();
        try {
            
            image = ImageIO.read(imageFile);
            for(int i = 0; i < image.getHeight(); i++)
                for(int j = 0; j < image.getWidth(); j++)
                {
                    pixel = image.getRGB(j, i);
                    if(pixel == -1)
                        pixelList.add(-1);
                    else
                        pixelList.add(1);
                }
            
            imageMap.put(key, pixelList);
            printPixelList(pixelList);
            
        } catch (IOException ex) {
            Logger.getLogger(ImageSet.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    public void printPixelList(List<Integer> pixelList){
        int i = 1;
        String output = "";
        char pixelChar;
        for(Integer pixel : pixelList)
        {
            if(pixel == -1)
                pixelChar = ' ';
            else
                pixelChar = '*';
                
            output += pixelChar;
            if((i % (imageWidth)) == 0)
                output += "\n";
            i++;
        }
        System.out.println(output);
    }
}
