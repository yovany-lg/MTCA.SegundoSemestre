/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package javadecisiontree;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.TreeMap;

/**
 *
 * @author yova
 */
public class NearestNeighbor {
    
    public static Double getMinkowskiDistance(List<Double> pattern1,List<Double> pattern2, Integer nAtt,Double p){//[class,att1,att2,...]
        //Con p = 2 => Distancia Euclidiana, con p = 1 => Distancia Manhattan
        Double sum = 0.0, distance;
        for(int i = 0; i < nAtt; i++){
            sum += Math.pow( Math.abs(pattern1.get(i+1) - pattern2.get(i+1)) , p);
        }
        distance = Math.pow(sum, 1.0/p);
        return distance;
    }
    
    public static Double getMinkowskiFinalDistance(List<Double> pattern1,List<Double> pattern2, Integer nAtt,Double p){//[class,att1,att2,...]
        //Con p = 2 => Distancia Euclidiana, con p = 1 => Distancia Manhattan
        Double sum = 0.0, distance;
        for(int i = 0; i < nAtt; i++){
            sum += Math.pow( Math.abs(pattern1.get(i+1) - pattern2.get(i)) , p);
        }
        distance = Math.pow(sum, 1.0/p);
        return distance;
    }

    public static Double NNClass(TrainingSet trainingSet, List<Double> pattern,Integer k){
        Map<Integer,Double> patternDistanceMap = new HashMap<Integer,Double>(); //[patternKey => distance]
        Double distance;
        for( Map.Entry<Integer, List<Double>> entry : trainingSet.trainingSet.entrySet() )
        {
            distance = getMinkowskiFinalDistance(entry.getValue(),pattern,trainingSet.nAtt,2.0);
            patternDistanceMap.put(entry.getKey(), distance );//Distancia Euclidiana
        }
        DistanceComparator bvc =  new DistanceComparator(patternDistanceMap);
        Map<Integer,Double> pKeyDistanceMap = new TreeMap<Integer,Double>(bvc);
        pKeyDistanceMap.putAll(patternDistanceMap);//[patternKey => distance], Se ordenan las claves de los ejemplos en base a la distancia
        return getClassByKnn(trainingSet,pKeyDistanceMap,k);
    }
    
    public static Double getClassByKnn(TrainingSet trainingSet,Map<Integer,Double> pKeyDistanceMap,Integer k){
        int i = 0;
        int positiveExamples = 0, negativeExamples = 0;
        for(Map.Entry<Integer,Double> entry : pKeyDistanceMap.entrySet()){
            if( i < k )
                if( Double.compare(trainingSet.trainingSet.get(entry.getKey()).get(0), trainingSet.positiveClass) ==0 )
                    positiveExamples++;
                else
                    negativeExamples++;
            else
                break;
            i++;
        }
        
        if(negativeExamples > positiveExamples)
            return trainingSet.negativeClass;
        else
            return trainingSet.positiveClass;
    }
    
    public static Double testValidationSet(TrainingSet trainingSet,TrainingSet validationSet, Integer k){
        Integer empiricalError = 0;  
        Double errorRate;
        for( Map.Entry<Integer, List<Double>> entry : validationSet.trainingSet.entrySet() )
        {
            if(!testPattern(trainingSet,entry.getValue(),k))
                empiricalError++;
        }
        
        errorRate = ((double)empiricalError)/((double)validationSet.trainingSet.size());
        return errorRate;
    }
    
    public static boolean testPattern(TrainingSet trainingSet, List<Double> pattern,Integer k){
        Double nnClass;
        nnClass = NNClass(trainingSet, pattern, k);
        if(Double.compare(pattern.get(0),nnClass) == 0)
            return true;
        else
            {
                //Si la clasificacion correcta (del ejemplo) es positiva, entonces se tiene un Negativo Falso
                if(Double.compare(pattern.get(0),1.0) == 0)
                    System.out.println("Negativo Falso");
                else
                    System.out.println("Positivo Falso");
                return false;
            
            }
    }

    public static String testFinalPattern(TrainingSet trainingSet, List<Double> pattern,Integer k){
        Double nnClass;
        normaliseTestPattern(trainingSet.meanSDByAtt,pattern);
        nnClass = NNClass(trainingSet, pattern, k);
        //clase positiva, 1.0 => Maligna. Clase negativa: 2.0 => Benigna
        if(Double.compare(nnClass,1.0) == 0)
            return "Maligna";
        else
            return "Benigna";
    }
    
    public static void normaliseTestPattern(Map<Integer,List<Double>> meanSDByAtt,List<Double> pattern){
        Double normalizedValue;
        for (int i = 0; i < pattern.size(); i++) {
            normalizedValue = (pattern.get(i) - meanSDByAtt.get(i).get(0))/meanSDByAtt.get(i).get(1);
            pattern.set(i, normalizedValue);
	}
    }
}
