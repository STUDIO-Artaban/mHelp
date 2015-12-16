/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package mHpJavaCtrl;

import java.awt.Component;
import java.awt.Frame;
import java.lang.reflect.Field;
import javax.swing.JFrame;

/**
 *
 * @author Pascal
 */
public abstract class mHpJavaCtrl<CtrlClass> {
    
    protected final int NO_VALUE = -1;
    
    /*
     * Declarations
     */
    private CtrlClass CtrlObj = null;
    
    protected String strJarVersion = null;
    protected String strCtrlClass = null;
    protected String[] pCtrlName = null;
    
    protected int iCtrlStyle = NO_VALUE;
    
    /*
     * GetJarVersion
     */
    public abstract String GetJarVersion();
    
    /*
     * GetCtrlClass
     */
    public abstract String GetCtrlClass(int iClassIdx);
    
    /*
     * GetCtrlName
     */
    public abstract String GetCtrlName(int iNameIdx);
        
    /*
     * getInstance
     */
    public void getInstance(CtrlClass CtrlInst) {
        
        // Assign control object instance
        if (CtrlObj == null) {
            CtrlObj = CtrlInst;
        }
    }
    
    /*
     * setmHpName
     */
    private boolean setmHpName(Frame Parent) {

        Component[] ChildComp = ((JFrame)Parent).getContentPane().getComponents();
        if (ChildComp == null) {
            return false;
        }
        
        // Get name of control class declared in the parent component
        for(Field field : Parent.getClass().getDeclaredFields()) {

            // Check if field class match with control class
            if (field.getType().isInstance(CtrlObj)) {
                
                try {
                    
                    // Get object from field
                    field.setAccessible(true);
                    CtrlClass Ctrl = (CtrlClass)field.get(Parent);
                    
                    // Loop on all component defined in the parent component
                    for (int i=0; i < ChildComp.length; i++) {
                        
                        // Check same control object class
                        if (ChildComp[i].getClass() == CtrlObj.getClass()) {

                            // Check same control instance
                            CtrlClass CtrlComp = (CtrlClass)ChildComp[i];
                            if (Ctrl.equals(CtrlComp)) {

                                // Set control name if empty
                                if ((ChildComp[i].getName() == null) ||
                                    (ChildComp[i].getName().isEmpty())) {
                                    ChildComp[i].setName(field.getName());
                                }
                            }
                        }
                    }
                    field.setAccessible(false);
                }
                catch (IllegalAccessException ie) {
                    continue;
                }
            }
        }
        return true;
    }
    
    /*
     * AddmHpEvent
     */
    public abstract boolean AddmHpEvent(Frame Parent);
    
    /*
     * AddJavaEvents
     */
    public boolean AddJavaEvents(boolean bCreatedVM) {

        Frame[] aFrame = JFrame.getFrames();
        
        // Get active frame
        int iFrm = 0, iActiveFrm = -1;
        for(Frame CurFrame : aFrame) {
            if (CurFrame.isActive()) {
                if (!setmHpName(CurFrame)) {
                    return false;
                }
                iActiveFrm = iFrm;
                break;
            }
            iFrm++;
        }
        
        // Check existing active Frame
        if (iActiveFrm < 0) {
            if (bCreatedVM) {
                return true; // Not a Java application
            }
            return false;
        }
        
        // Add mHelp control events
        return AddmHpEvent(aFrame[iActiveFrm]);
    }
}
