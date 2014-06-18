/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package javadecisiontree;

import java.util.Comparator;
import java.util.List;
import java.util.Map;

/**
 *
 * @author yova
 */
class ValueComparator implements Comparator<Integer> {

    Map<Integer,List<Double>> base;
    public ValueComparator(Map<Integer,List<Double>> base) {
        this.base = base;
    }

    // Note: this comparator imposes orderings that are inconsistent with equals.    
    public int compare(Integer a, Integer b) {
        if (base.get(a).get(1) < base.get(b).get(1)) {
            return -1;
        } else {
            return 1;
        } // returning 0 would merge keys
    }
}