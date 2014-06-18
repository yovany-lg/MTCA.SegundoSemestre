/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package javadecisiontree;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Formatter;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Level;
import java.util.logging.Logger;
/**
 *
 * @author yova
 */
public class DTLearning {
//    private double [][] attValues; 

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException, IOException {
        // TODO code application logic here
        TrainingSet trainingSet = new TrainingSet("..\\excelFiles\\11_Caracteristicas.xls",58,11);
        TrainingSet testSet = new TrainingSet(args[0],1, 11);
        trainingSet.normalization();
//------------NN FINAL TEST
        System.out.println(NearestNeighbor.testFinalPattern(trainingSet, testSet.trainingSet.get(0), 5));

//        //System.out.println(args[0]);
//        
//        
        
        
//        Double avgErrorRateKnn,avgPredictiveAccuracyKnn, avgErrorRateDT,avgPredictiveAccuracyDT;
//
//        avgErrorRateDT = CrossValidation(trainingSet,58);
//        avgPredictiveAccuracyDT = 1.0 - avgErrorRateDT;
//        System.out.println("Average Error Rate: "+avgErrorRateDT+"\nAverage Predictive Accuracy: "+avgPredictiveAccuracyDT);

//        avgErrorRateKnn = nnCrossValidation(trainingSet, 58);
//        avgPredictiveAccuracyKnn = 1.0 - avgErrorRateKnn;
//        System.out.println("Average Error Rate: "+avgErrorRateKnn+"\nAverage Predictive Accuracy: "+avgPredictiveAccuracyKnn);
        
        
//        DecisionTree rootNode = new DecisionTree();
//        rootNode = learningAlgorithm(trainingSet);
//        rootNode.saveTreeFile(rootNode, "decisionTree.dat");

//------------DecisionTree FINAL TEST
//        trainingSet.localDiscretisation();
//        DecisionTree rootNode2 = new DecisionTree();
//        rootNode2 = rootNode2.retrieveTreeFile("..\\decisionTree.dat");
//        rootNode2.normaliseTestPattern(trainingSet.meanSDByAtt, testSet.trainingSet.get(0));
//        System.out.println(rootNode2.testFinalPattern(testSet.trainingSet.get(0)));
        
        return;
    }
    
    public static DecisionTree learningAlgorithm(TrainingSet trainingSet){
        DecisionTree rootNode = new DecisionTree();
        TrainingSet parent = null;
        trainingSet.localDiscretisation();
        learningAlgorithm(trainingSet,parent,rootNode);
        return rootNode;
    }
    
    public static void learningAlgorithm(TrainingSet trainingSet,
            TrainingSet parentTrainingSet,DecisionTree currentNode){
        if(trainingSet.trainingSet.isEmpty())
        {
            currentNode.decision = (parentTrainingSet.pluralityValue());//Es un nodo hoja
        }else{
            double clasification = trainingSet.sameClasification();
            if(clasification != -1)
            {
                currentNode.decision= clasification; //es un nodo hoja
            }else
                if(trainingSet.discreteAtt.isEmpty())
                {
                    currentNode.decision = trainingSet.pluralityValue();//Es un nodo hoja
                }else{
                    List<TrainingSet> setList;
                    int att;
                    
                    att = newDecisionNode(trainingSet,currentNode);
                    setList = trainingSet.divideSetOnAtt(att);

                    currentNode.left = new DecisionTree();
                    currentNode.right = new DecisionTree();

                    
                    learningAlgorithm(setList.get(0),trainingSet,currentNode.left);
                    learningAlgorithm(setList.get(1),trainingSet,currentNode.right);
                }
        }
    }
    
    public static int newDecisionNode(TrainingSet trainingSet, DecisionTree currentNode){
        int moreImportantAtt = -1;
        double moreImportant = 0.0, currentImportance;
        for(Map.Entry<Integer,Double> entry : trainingSet.discreteAtt.entrySet())
        {
            if(moreImportantAtt == -1)
                moreImportantAtt = entry.getKey();
            currentImportance = trainingSet.finalAttGain(entry.getKey());
            if(moreImportant < currentImportance)
            {
                moreImportant = currentImportance;
                moreImportantAtt = entry.getKey();
            }
        }
        
        currentNode.attribute = moreImportantAtt;
        currentNode.decision = trainingSet.discreteAtt.get(moreImportantAtt);//Se obtiene el umbral
        return moreImportantAtt;
    }
    
    public static Double CrossValidation(TrainingSet trainingSet, Integer k ){
        Double validationErrorRate = 0.0;
        trainingSet.normalization();

        DecisionTree rootNode;// = new DecisionTree();
        rootNode = learningAlgorithm(trainingSet);
//        rootNode.saveTreeFile(rootNode, "decisionTree.dat");
        getTreeImage(rootNode,0);
        for(int fold = 1; fold <= k; fold++){
            List<TrainingSet> setPartition;//[trainingSet,validationSet]
            setPartition = trainingSet.kFoldPartition(k,fold);
            validationErrorRate +=rootNode.testValidationSet(setPartition.get(1));
        }
        
        return validationErrorRate/((double)k);
    }

    public static Double nnCrossValidation(TrainingSet trainingSet, Integer k ){
        Double validationErrorRate = 0.0;
        trainingSet.normalization();
        for(int fold = 1; fold <= k; fold++){
            List<TrainingSet> setPartition;//[trainingSet,validationSet]
            setPartition = trainingSet.kFoldPartition(k,fold);
            validationErrorRate += NearestNeighbor.testValidationSet(setPartition.get(0),setPartition.get(1),5);
        }
        
        return validationErrorRate/((double)k);
    }
    
    public static void getTreeImage(DecisionTree rootNode,Integer fold){
        String file;
        file = rootNode.getDotFile();

        Formatter output;
        try {
            output = new Formatter("layoutFiles\\"+fold+"FoldTree.dot");
            output.format("%s", file);
            output.close();

            String command2 = "\"C:\\Program Files (x86)\\Graphviz2.38\\bin\\dot.exe\" layoutFiles\\"+fold+"FoldTree.dot | "
                    + "\"C:\\Program Files (x86)\\Graphviz2.38\\bin\\gvpr.exe\" -c -f layoutFiles\\tree.gv | "
                    + "\"C:\\Program Files (x86)\\Graphviz2.38\\bin\\neato\" -n -Tpng -o layoutFiles\\"+fold+"FoldTree.png";                

            String[] command =
            {
                "cmd",
            };
            Process p = Runtime.getRuntime().exec(command);
            new Thread(new SyncPipe(p.getErrorStream(), System.err)).start();
            new Thread(new SyncPipe(p.getInputStream(), System.out)).start();
            PrintWriter stdin = new PrintWriter(p.getOutputStream());
            stdin.println(command2);
            // write any other commands you want here
            stdin.close();
            int returnCode = p.waitFor();
            //System.out.println("Return code = " + returnCode);                

//                String command ="cmd c/ dot layoutFiles\\"+fold+"FoldTree.dot | gvpr -c -f layoutFiles\\tree.gv | neato -n -Tpng -o layoutFiles\\"+fold+"FoldTree.png";
//                String[] commands = new String[]{"dot", "layoutFiles\\"+fold+"FoldTree.dot | gvpr -c -f layoutFiles\\tree.gv | neato -n -Tpng -o layoutFiles\\"+fold+"FoldTree.png"};
//                Process child = Runtime.getRuntime().exec(command);
        } catch (FileNotFoundException ex) {
            Logger.getLogger(DTLearning.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(DTLearning.class.getName()).log(Level.SEVERE, null, ex);
        } catch (InterruptedException ex) {
            Logger.getLogger(DTLearning.class.getName()).log(Level.SEVERE, null, ex);
        }
        //System.out.println(file);        
    }

//    public void localDiscretisation(TrainingSet trainingSet){
//        Map<Integer,Double> attDiscreteMap = new HashMap< Integer, Double >();;  //{'attID' => Umbral}
//        for(int i = 0; i< trainingSet.nAtt;i++){
//            attDiscreteMap.put(i,greaterAttUmbralGain(trainingSet,i));
//        }
//    }
    
//    public Double greaterAttUmbralGain(TrainingSet trainingSet,int i){
//        Map<Integer, List<Double>> attSortedMap = trainingSet.getAttSortedMap(i);
//        displayMap(attSortedMap);
//        return 0.0;
//    }
//    
//    private static void displayMap( Map<Integer, List<Double>> map )
//    {
//        Set< Integer > keys = map.keySet();
//        // sort keys
//        TreeSet< Integer > sortedKeys =new TreeSet< Integer >( keys );
//
//        System.out.println( "\nMap contains:\nKey\t\tValue" );
//
//        // generate output for each key in map
//        for( Integer key : sortedKeys )
//            System.out.printf("%-10s%10s\n", key,map.get(key));
//        System.out.printf("\nsize: %d\nisEmpty: %b\n",map.size(), map.isEmpty());
//    } // end method displayMap
    
}


//        try {
//            FileInputStream file = new FileInputStream(new File("excelFiles\\11_Caracteristicas.xls"));
//
//            //Get the workbook instance for XLS file 
//            HSSFWorkbook workbook = new HSSFWorkbook(file);
//
//            //Get first sheet from the workbook
//            HSSFSheet sheet = workbook.getSheetAt(0);
//
//            //Iterate through each rows from first sheet
//            Iterator<Row> rowIterator = sheet.iterator();
//            while(rowIterator.hasNext()) {
//                Row row = rowIterator.next();
//
//                //For each row, iterate through each columns
//                Iterator<Cell> cellIterator = row.cellIterator();
//                while(cellIterator.hasNext()) {
//
//                    Cell cell = cellIterator.next();
//
//                    switch(cell.getCellType()) {
//                        case Cell.CELL_TYPE_BOOLEAN:
//                            System.out.print(cell.getBooleanCellValue() + "\t\t");
//                            break;
//                        case Cell.CELL_TYPE_NUMERIC:
//                            System.out.print(cell.getNumericCellValue() + "\t\t");
//                            break;
//                        case Cell.CELL_TYPE_STRING:
//                            System.out.print(cell.getStringCellValue() + "\t\t");
//                            break;
//                    }
//                }
//                System.out.println("");
//            }
//            file.close();
//            FileOutputStream out = 
//                new FileOutputStream(new File("excelFiles\\11_Caracteristicas.xls"));
//            workbook.write(out);
//            out.close();
//
//        } catch (FileNotFoundException e) {
//            e.printStackTrace();
//        } catch (IOException e) {
//            e.printStackTrace();
//        }    