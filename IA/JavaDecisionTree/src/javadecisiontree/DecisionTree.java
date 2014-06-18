/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package javadecisiontree;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;
import java.util.Map;
import static javadecisiontree.pBinTreeIntegerWR.tree_FileRead;
import static javadecisiontree.pBinTreeIntegerWR.tree_FileWrite;

/**
 *
 * @author yova
 */
public class DecisionTree{

    /**
     *
     */
    protected double decision;//decision(si es nodo hoja) o umbral
    protected int attribute;//numero del atributo
    protected DecisionTree left;
    protected DecisionTree right;
    
    public DecisionTree() {
        attribute = -1;
        left = null;
        right = null;
    }
    
    public boolean isLeafNode(){
        if(this.left == null && this.right == null)
            return true;
        else
            return false;
    }
    
    public void saveTreeFile(DecisionTree rootNode,String fileName){
        File file = new File(fileName);//"pBinTreeIntegerWR.dat");
        try{
            writeTreeFile(rootNode,
                new DataOutputStream(
                new FileOutputStream(file)));
        }catch(IOException e){
            System.out.println(e);
        }
    }

    public DecisionTree retrieveTreeFile(String fileName){
        File file = new File(fileName);//"pBinTreeIntegerWR.dat");
        DecisionTree rootNode = null;
        try{
            rootNode = readTreeFile(rootNode,
                new DataInputStream(
                new FileInputStream(file)));
        }catch(IOException e){
            System.out.println(e);
        }
        return rootNode;
    }
    
    public void writeTreeFile(DecisionTree currentNode,DataOutputStream output) throws IOException{
        if(currentNode != null){
            output.writeBoolean(true);
            output.writeInt(currentNode.attribute);
            output.writeDouble(currentNode.decision);
            writeTreeFile(currentNode.left,output);
            writeTreeFile(currentNode.right,output);
        }else
            output.writeBoolean(false);
    }

    public DecisionTree readTreeFile(DecisionTree currentNode,DataInputStream input) throws IOException{
        if(input.readBoolean()){
            currentNode = new DecisionTree();
            currentNode.attribute = new Integer(input.readInt());
            currentNode.decision = new Double(input.readDouble());
            currentNode.left = readTreeFile(currentNode.left,input);
            currentNode.right = readTreeFile(currentNode.right,input);
        }else
            currentNode = null;
        return currentNode;
    }    
    
    public Double testValidationSet(TrainingSet trainingSet){
        Integer empiricalError = 0;  
        Double errorRate;
        for( Map.Entry<Integer, List<Double>> entry : trainingSet.trainingSet.entrySet() )
        {
            if(!testPattern(entry.getValue()))
                empiricalError++;
        }
        
        errorRate = ((double)empiricalError)/((double)trainingSet.trainingSet.size());
        return errorRate;
    }
    
    public boolean testPattern(List<Double> pattern){   //[Class,att1,att2,...]
        if(isLeafNode()) 
            if(Double.compare(pattern.get(0),decision) == 0)  //Si se obtiene la clasificacion correcta...
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
        
        if(pattern.get(attribute+1) < decision)  //Menor que el umbral para el atributo actual
            return this.left.testPattern(pattern);
        else
            return this.right.testPattern(pattern);
    }
    
    public String getDotFile(){
      StringBuilder sb = new StringBuilder();
      sb.append("digraph G {\n");
      sb.append("graph [ dpi = 200 ]\n"); 
      sb.append("nodesep=0.3;\n");
      sb.append("ranksep=0.2;\n");
      sb.append("margin=0.1;\n");
      sb.append("node [shape=circle];\n");
      sb.append("edge [arrowsize=0.8];\n");

      StringBuilder treeContent = getDotTreeContent(new StringBuilder(),this, 1);
      sb.append(treeContent);

      sb.append("}");

      return sb.toString();
    }

    // Pre: N is not null.
    // This won't work for larger unbalanced trees (int overflow), but then again you probably
    // wouldn't be displaying them anyway, so this is good enough for now.
    private StringBuilder getDotTreeContent(StringBuilder sb, DecisionTree currentNode, int i){
        if(currentNode.isLeafNode())
            //clase positiva, 1.0 => Maligna. Clase negativa: 2.0 => Benigna
            if(Double.compare(currentNode.decision,1.0) == 0)
                sb.append(String.format("node%d [label=\"Maligna\"];\n", i));
            else
                sb.append(String.format("node%d [label=\"Benigna\"];\n", i));
        else
            sb.append(String.format("node%d [label=\"Att%d\"];\n", i, currentNode.attribute));
            
      int lChild = 2*i;
      int rChild = 2*i + 1;

      if(currentNode.left  != null){
        sb.append(String.format("node%d -> node%d;\n", i, lChild));
        getDotTreeContent(sb, currentNode.left, lChild);
      }
      if(currentNode.right != null){
        sb.append(String.format("node%d -> node%d;\n", i, rChild));
        getDotTreeContent(sb, currentNode.right, rChild);
      }
      return sb;
    }

}
