/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package hopfield;

/**
 *
 * @author yova
 */
public class Hopfield {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        String imageSetPath = "C:\\Users\\yova\\Documents\\GitHub\\MTCA.SegundoSemestre\\ANN\\Hopfield\\imageSet\\";
        String testImageSetPath = "C:\\Users\\yova\\Documents\\GitHub\\MTCA.SegundoSemestre\\ANN\\Hopfield\\testImageSet\\";
        ImageSet imageSet = new ImageSet(10,13);
        ImageSet testImageSet = new ImageSet(10,13);
        System.out.println("Patrones de Aprendizaje\n----------------------------\n");
//        imageSet.addImage(0,imageSetPath+"A11.PNG");
//        imageSet.addImage(1,imageSetPath+"B11.PNG");
//        imageSet.addImage(2,imageSetPath+"C11.PNG");
//        imageSet.addImage(3,imageSetPath+"D11.PNG");
//        imageSet.addImage(4,imageSetPath+"E11.PNG");
        imageSet.addImage(5,imageSetPath+"F11.PNG");
        imageSet.addImage(6,imageSetPath+"G11.PNG");
        imageSet.addImage(7,imageSetPath+"H11.PNG");
//        imageSet.addImage(8,imageSetPath+"I11.PNG");
//        imageSet.addImage(9,imageSetPath+"J11.PNG");
        imageSet.addImage(10,imageSetPath+"K11.PNG");
//        imageSet.addImage(11,imageSetPath+"L11.PNG");
//        imageSet.addImage(12,imageSetPath+"M1.PNG");
//        imageSet.addImage(13,imageSetPath+"N1.PNG");
//        imageSet.addImage(14,imageSetPath+"O1.PNG");
//        imageSet.addImage(15,imageSetPath+"P1.PNG");
//        imageSet.addImage(16,imageSetPath+"Q1.PNG");
//        imageSet.addImage(17,imageSetPath+"R1.PNG");
//        imageSet.addImage(18,imageSetPath+"S1.PNG");
//        imageSet.addImage(19,imageSetPath+"T1.PNG");
//        imageSet.addImage(20,imageSetPath+"U1.PNG");
//        imageSet.addImage(21,imageSetPath+"V1.PNG");
//        imageSet.addImage(22,imageSetPath+"W1.PNG");
//        imageSet.addImage(23,imageSetPath+"X1.PNG");
//        imageSet.addImage(24,imageSetPath+"Y1.PNG");
//        imageSet.addImage(25,imageSetPath+"Z1.PNG");
        System.out.println("----------------------------\nPatron de Test...\n");
        testImageSet.addImage(0, testImageSetPath+"F22.PNG");
        HopfieldModel hopfieldModel = new HopfieldModel(10*13);
        hopfieldModel.learn(imageSet);
        hopfieldModel.retrieve(testImageSet.imageMap.get(0));
        
    }
    
}
