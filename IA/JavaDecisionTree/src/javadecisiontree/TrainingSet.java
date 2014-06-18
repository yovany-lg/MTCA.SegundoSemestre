/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package javadecisiontree;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;

/**
 *
 * @author yova
 */
public class TrainingSet {
    public String xlsFileName;
    public int nPatt, nAtt; //Patronoes, Atributos
    public Map<Integer, List<Double>> trainingSet = new HashMap<Integer,List<Double>>();
    public ArrayList<String> attLabels = new ArrayList<String>();
    public double positiveClass = 1.0, negativeClass = 2.0 ;  //clase positiva, 1.0 => Maligna. Clase negativa: 2.0 => Benigna
    public Map<Integer, Double> discreteAtt = new HashMap<Integer, Double>(); //Valores de umbral para atributos: 'attID' => Umbral
    public Map<Integer,List<Double>> meanSDByAtt =  new HashMap<Integer,List<Double>>();
    
    
    public TrainingSet(String fileName,int n,int at){
        xlsFileName = fileName;
        nPatt= n;
        nAtt = at;
        //trainingSet = newMap(nPatt,nAtt+1);//En la primer posicion esta la clasificacion
        getTrainingSet();
    }

    public TrainingSet(TrainingSet other){
        xlsFileName = other.xlsFileName;
        nPatt= other.nPatt;
        nAtt = other.nAtt;
        trainingSet.putAll(other.trainingSet);//En la primer posicion esta la clasificacion
        attLabels.addAll(other.attLabels);
//        positiveClass = other.positiveClass;
//        negativeClass = other.negativeClass;
        discreteAtt.putAll(other.discreteAtt);
//        getTrainingSet();
    }
    
    public double sameClasification(){
        int positiveCases =0;
        int negativeCases =0;

        for( Map.Entry<Integer, List<Double>> entry : trainingSet.entrySet() )
        {
            if(entry.getValue().get(0) == positiveClass)  //Clase positiva
                positiveCases++;
            else
                negativeCases++;
        }
        
        if(negativeCases == 0 )
            return positiveClass;
        if(positiveCases == 0)
            return negativeClass;
        return -1.0;
    }

    public double pluralityValue(){
        int positiveCases =0;
        int negativeCases =0;

        for( Map.Entry<Integer, List<Double>> entry : trainingSet.entrySet() )
        {
            if(entry.getValue().get(0) == positiveClass)  //Clase positiva
                positiveCases++;
            else
                negativeCases++;
        }
        
        if(negativeCases <= positiveCases)
            return positiveClass;
        else
            return negativeClass;
    }
    
    public List<TrainingSet> divideSetOnAtt(int i){//[set1,set2] set1 before umbral set2 after umbral
        List<TrainingSet> list = new ArrayList<TrainingSet>();
        TrainingSet set1 = new TrainingSet(this), set2 = new TrainingSet(this);
        for( Map.Entry<Integer, List<Double>> entry : trainingSet.entrySet() )
        {
            if(entry.getValue().get(i+1) < discreteAtt.get(i))  //si esta antes del umbral
                set2.trainingSet.remove(entry.getKey());
            else
                set1.trainingSet.remove(entry.getKey());
        }
        set1.discreteAtt.remove(i);
        set2.discreteAtt.remove(i);
        list.add(set1);
        list.add(set2);
        return list;
    }
 
    public List<TrainingSet> kFoldPartition(Integer k, Integer fold){
        Integer foldSize,i = 0;
        foldSize = trainingSet.size() / k;
        TrainingSet set1 = new TrainingSet(this), set2 = new TrainingSet(this);
        List<TrainingSet> list = new ArrayList<TrainingSet>();

        for( Map.Entry<Integer, List<Double>> entry : trainingSet.entrySet() )
        {
            if( i >= (fold-1)*foldSize && i < (fold)*foldSize  )  //si forma parte del fold
                set1.trainingSet.remove(entry.getKey()); //trainingSet
            else
                set2.trainingSet.remove(entry.getKey());    //validationSet
            i++;
        }
        list.add(set1);
        list.add(set2);
        return list;
    }
    
    //Moverla de aqui
    public void getTrainingSet(){
        int i = 0;
        try {
            FileInputStream file = new FileInputStream(new File(xlsFileName));
            //Get the workbook instance for XLS file 
            HSSFWorkbook workbook = new HSSFWorkbook(file);
            //Get first sheet from the workbook
            HSSFSheet sheet = workbook.getSheetAt(0);
            //Iterate through each rows from first sheet
            Iterator<Row> rowIterator = sheet.iterator();
            
            while(rowIterator.hasNext()) {
                Row row = rowIterator.next();
                if(i == 0){
                    getAttLabels(row);
                    i++;
                }
                else
                {
                    getPattern(row,i-1);//Agregar el patron al conjunto de entrenamiento
                    i++;
                }
            }
            file.close();
            //FileOutputStream out = new FileOutputStream(new File(xlsFileName));
            //workbook.write(out);
            //out.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
        
    //Generar el mapa dinamico para la matriz de patrones de entrenamiento
//    public static Map<Integer, List<Double>> newMap(Integer row, Integer column) {
//        Map<Integer, List<Double>> doubleMap = new HashMap<Integer, List<Double>>();
//
//        for (int x = 0; x < row; x++) {
//            for (int y = 0; y < column; y++) {
//                doubleMap.put(x, new ArrayList<Double>());
//            }
//        }
//        return doubleMap;
//    }
    
    public void getAttLabels(Row row){
        int i = 0;
        //For each row, iterate through each columns
        Iterator<Cell> cellIterator = row.cellIterator();

        while(cellIterator.hasNext()) {
            Cell cell = cellIterator.next();
            if(i>0)     //Las clases vienen en la primer columna       
                attLabels.add(cell.getStringCellValue());
            i++;
        }        
    }

    public void getPattern(Row row,int iPatt){
        List<Double> pattern = new ArrayList<Double>();
        //For each row, iterate through each columns
        Iterator<Cell> cellIterator = row.cellIterator();

        while(cellIterator.hasNext()) {
            Cell cell = cellIterator.next();
            pattern.add(cell.getNumericCellValue()); //Clasificacion en la primer columna
        }  
        trainingSet.put(iPatt, pattern);
    }
    
    public TreeMap<Integer,List<Double>> getAttSortedMap(int att){
        Map<Integer,List<Double>> attMap = new HashMap<Integer,List<Double>>();
        for( Map.Entry<Integer, List<Double>> entry : trainingSet.entrySet() )
        {
            List<Double> pattern = new ArrayList<Double>();        
            pattern.add(entry.getValue().get(0)); //Clasificacion
            pattern.add(entry.getValue().get(att+1)); //Atributo
            attMap.put(entry.getKey(), pattern);
            pattern = null;
        }
        ValueComparator bvc =  new ValueComparator(attMap);
        TreeMap<Integer,List<Double>> attSortedMap = new TreeMap<Integer,List<Double>>(bvc);
        attSortedMap.putAll(attMap);
        return attSortedMap;
    }
    
    public void localDiscretisation(){
        for(int i =0; i< nAtt; i++)
        {
            discreteAtt.put(i,getAttUmbral(i));
        }
    }

    public Double getAttUmbral(int i){
        TreeMap<Integer, List<Double>> attSortedMap = getAttSortedMap(i);
        double gain,maxGain = 0.0,value,bestUmbral = 0.0;
        Map.Entry<Integer, List<Double>> prevEntry = null;

        for( Map.Entry<Integer, List<Double>> entry : attSortedMap.entrySet() )
        {
            if(prevEntry != null)
            {
                value = entry.getValue().get(1);
                if(prevEntry.getValue().get(1) < value)
                {
                    gain = getAttGain(attSortedMap,value);
                    if(gain > maxGain)
                    {
                        maxGain = gain;
                        bestUmbral = value;
                        //System.out.print("Key: "+entry.getKey()+" => Value: "+bestUmbral+", Gain: "+maxGain+'\n');                
                    }
                }
            }
            prevEntry = entry;
        }
        //System.out.print('\n');
        return bestUmbral;
    }
    
    public Double getAttGain(Map<Integer, List<Double>> attSortedMap,Double umbral){        
        return getWholeSetEntropy(attSortedMap) - getRemainder(attSortedMap,umbral);
    }
    
    public Double finalAttGain(int i){
        Map<Integer, List<Double>> attSortedMap = getAttSortedMap(i);
        double umbral = discreteAtt.get(i);
        return getWholeSetEntropy(attSortedMap) - getRemainder(attSortedMap,umbral);
    }
    
    public Double getWholeSetEntropy(Map<Integer, List<Double>> attSortedMap){
        //la clase 2 es para los patrones benignos, la clase 1 para los patrones malignos
        //la clase 1 se considerara como positivo para el calculo de la entropia    
        //double entropy = getBooleanEntropy(getPositiveCases(attSortedMap)/((double)attSortedMap.size()));
        return getBooleanEntropy(getPositiveCases(attSortedMap)/((double)attSortedMap.size()));
    }
    
    public Double getRemainder(Map<Integer, List<Double>> attSortedMap,Double umbral){
        //la clase 2 es para los patrones benignos, la clase 1 para los patrones malignos
        //la clase 1 se considerara como positivo para el calculo de la entropia
        double beforeUmbralCases = 0, beforeUmbralPositiveCases =0;
        double afterUmbralCases = 0, afterUmbralPositiveCases =0;
        double patternCount = (double)attSortedMap.size();

        for( Map.Entry<Integer, List<Double>> entry : attSortedMap.entrySet() )
        {
            if(entry.getValue().get(1) < umbral)
            {
                beforeUmbralCases++;//Clasificacion en la primer columna
                if(entry.getValue().get(0) == positiveClass)
                    beforeUmbralPositiveCases++;
            }else
            {
                afterUmbralCases++;//Clasificacion en la primer columna
                if(entry.getValue().get(0) == positiveClass)
                    afterUmbralPositiveCases++;
            }
        }        
        
        return (beforeUmbralCases/patternCount)*getBooleanEntropy(beforeUmbralPositiveCases/beforeUmbralCases) +
                (afterUmbralCases/patternCount)*getBooleanEntropy(afterUmbralPositiveCases/afterUmbralCases);
    }

    public Double getPositiveCases(Map<Integer, List<Double>> attSortedMap){
        int positiveCases = 0;
        for( Map.Entry<Integer, List<Double>> entry : attSortedMap.entrySet() )
            if(entry.getValue().get(0) == positiveClass)
                positiveCases++;//Clasificacion en la primer columna
        return (double)positiveCases;
    }
    
    public Double getBooleanEntropy(Double q){
        if( q == 0.0 || q == 1)
            return 0.0;
        else
            return -(q*logBased(q,2.0) + (1-q)*logBased((1-q),2.0));
    }
    
    public static double logBased( double a, double b )
    {
        return Math.log(a) / Math.log(b);
    }
        
    public void normalization(){
//        trainingSet.values().
        for( int i = 0; i < nAtt; i++ )
            normalizationByAtt(i);
    }

    public void normalizationByAtt(int att){
        Double normalizedValue, mean, stDev;
        List<Double> pattern = null;
        List<Double> meanSD = new ArrayList<Double>();
        mean = getMeanByAtt(att);
        stDev = getStDevByAtt(mean, att);
        meanSD.add(mean);
        meanSD.add(stDev);
        meanSDByAtt.put(att, meanSD);
        for( Map.Entry<Integer, List<Double>> entry : trainingSet.entrySet() ){
            pattern = entry.getValue();
            normalizedValue = (pattern.get(att+1) - mean)/stDev;
            pattern.set(att+1, normalizedValue);
            trainingSet.put(entry.getKey(), pattern);
        }
    }
    
    public Double getMeanByAtt(int att){
        Double sum = 0.0;
        for( Map.Entry<Integer, List<Double>> entry : trainingSet.entrySet() ){
            sum += entry.getValue().get(att+1);
        }
        
        return sum/trainingSet.size();
    }
    
    public Double getStDevByAtt(Double mean, int att){//Desviacion estantard
        Double sum = 0.0;
        for( Map.Entry<Integer, List<Double>> entry : trainingSet.entrySet() ){
            sum += Math.pow(mean-entry.getValue().get(att+1), 2);
        }
        
        return Math.sqrt(sum/trainingSet.size());
    }
}

//                //For each row, iterate through each columns
//                Iterator<Cell> cellIterator = row.cellIterator();
//                
//                while(cellIterator.hasNext()) {
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