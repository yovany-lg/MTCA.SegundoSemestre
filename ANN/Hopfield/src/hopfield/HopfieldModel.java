/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package hopfield;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

/**
 *
 * @author yova
 */
public class HopfieldModel {
    Map<Integer, List<Double>> weightMatrix = new HashMap<Integer,List<Double>>();
    Map<Integer,Double> stateVector = new HashMap<Integer,Double>();

    
    int neurons;
    
    public HopfieldModel(Integer neurons){
        this.neurons = neurons;
    }
    
    public void learn(ImageSet imageSet){
        for(int i = 0; i < neurons; i++)
            weightMatrix.put(i, weightVectorLearning(imageSet,i));
        
        return;
    }
    
    public List<Double> weightVectorLearning(ImageSet imageSet,int neuron){
        List<Double> weightVector = new ArrayList<Double>();
        for(int i = 0; i < neurons; i++){
            Double sum = 0.0;
            if(neuron != i){
                for(Map.Entry<Integer, List<Integer>> entry : imageSet.imageMap.entrySet()){
                    sum += (double)(entry.getValue().get(neuron)*entry.getValue().get(i));
                }
            }
            
            sum = sum/(double)neurons;
            weightVector.add(sum);
        }
        return weightVector;
    }
    
    public void retrieve(List<Integer> inputVector){
        initializeStateVector(inputVector);
        //printStateVector();
        int i = 1;
        System.out.println("Iteración: "+1);
        while(convergenceIteration() != 0)
        {
//            stateVector.remove(i);
            System.out.println("Iteración: "+(i+1));
            i++;
        }
        System.out.println("\nNo hay mas cambios en el vector de estados...\n");
    }
    
    public void initializeStateVector(List<Integer> inputVector){
        for(int i = 0; i < neurons; i++)
        {
            stateVector.put(i, (double)inputVector.get(i));
        }
    }
    
    public Integer convergenceIteration(){
        Random random = new Random();
        List<Integer> keys = new ArrayList<Integer>(stateVector.keySet());
        //Map<Integer,Double> newStateVector = new HashMap<Integer,Double>();
        int randKey,key,changes = 0;
        Double signumValue, localField;
        while(!keys.isEmpty())
        {
            randKey = random.nextInt(keys.size());
            key = keys.get(randKey);
            localField = localField(key);
            signumValue = signum(localField);
            if(Double.compare(signumValue ,0.0) != 0)
                if(Double.compare(signumValue, stateVector.get(key)) != 0)
                {
                    changes++;
                    stateVector.put(key, signumValue);                
                }
            keys.remove(randKey);
        }
        //stateVector.putAll(newStateVector);
        printStateVector();
        return changes;
    }
    
    public Double localField(int neuron){
        Double sum = 0.0;
        int i = 0;
        for(Double weight : weightMatrix.get(neuron)){
            sum += weight*stateVector.get(i);
            i++;
        }
        
        return sum;
    }
    
    public Double signum(Double localField){
        if(Double.compare(localField, 0.0) < 0)
            return -1.0;
        if(Double.compare(localField, 0.0) > 0)
            return 1.0;
        
        return 0.0;
    }
    
    public void printStateVector(){
        int i = 1;
        String output = "";
        char pixelChar;
        for(int j = 0; j < neurons; j++)
        {
            if(stateVector.get(j) == -1)
                pixelChar = ' ';
            else
                pixelChar = '*';
                
            output += pixelChar;
            if((i % (10)) == 0)
                output += "\n";
            i++;
        }
        System.out.println(output);
    }
    
    public void printStateVector(Map<Integer,Double> newStateVector){
        int i = 1;
        String output = "";
        char pixelChar;
        for(int j = 0; j < neurons; j++)
        {
            if(newStateVector.get(j) == -1)
                pixelChar = ' ';
            else
                pixelChar = '*';
                
            output += pixelChar;
            if((i % (10)) == 0)
                output += "\n";
            i++;
        }
        System.out.println(output);
    }
}
