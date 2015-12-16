/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package mHpJavaJButton;

import mHpJavaCtrl.mHpJavaCtrl;
import java.awt.Frame;
import javax.swing.JButton;

/**
 *
 * @author Pascal
 */
public class mHpJavaJButton extends mHpJavaCtrl<JButton> {
    
    /*
     * JButton style(s)
     */
    protected final int JBUTTON_STYLE_ID = 0;
    
    /*
     * mHpJavaCtrl.GetJarVersion
     */
    @Override
    public String GetJarVersion() {
        return strJarVersion;
    }
    
    /*
     * mHpJavaCtrl.GetCtrlClass
     */
    @Override
    public String GetCtrlClass(int iClassIdx) {
        
        // Return managed button class name (javax.swing.JButton)
        if (iClassIdx == 0) { // ...according valid class index
            return strCtrlClass;
        }
        return null;
    }
    
    /*
     * mHpJavaCtrl.GetCtrlName
     */
    @Override
    public String GetCtrlName(int iNameIdx) {
        
        // Return the name of the control...
        if (iNameIdx <= NO_VALUE) {
            
            // ...according control style
            if (iCtrlStyle == JBUTTON_STYLE_ID) {
                return pCtrlName[0]; // JButton
            }
            return null;
        }
        
        // ...according index parameter
        if (iNameIdx < 1) { // 1 control name is available
            return pCtrlName[iNameIdx]; // JButton
        }
        return null;
    }
    
    /*
     * mHpJavaCtrl.AddmHpEvent
     */
    @Override
    public boolean AddmHpEvent(Frame Parent) {
        
        // Add mHelp action event
        
        return true;
    }
    
    /*
     * mHpJavaJButton - Constructor
     */
    public mHpJavaJButton() {
        
        ////// WARNING: Always add code below
        getInstance(new JButton());
        
        //
        strJarVersion = "1.0.0.0";
        
        strCtrlClass = JButton.class.getCanonicalName();

        pCtrlName = new String[1];
        pCtrlName[0] = "JButton";
    }
}
