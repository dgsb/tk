/* $Id: ttkManager.h,v 1.1 2006/10/31 01:42:26 hobbs Exp $
 *
 * Copyright (c) 2005, Joe English.  Freely redistributable.
 *
 * Ttk widget set: Geometry management utilities.
 *
 * TODO: opacify data structures.
 */

#ifndef TTK_MANAGER_H
#define TTK_MANAGER_H 1

typedef struct TtkManager_ Ttk_Manager; /* forward */
typedef struct TtkSlave_ Ttk_Slave; /* forward */

/*
 * Geometry manager specification record:
 *
 * RequestedSize computes the requested size of the master window.
 *
 * PlaceSlaves sets the position and size of all managed slaves
 * by calling Ttk_PlaceSlave().
 *
 * SlaveAdded() is called after a new slave has been added.
 *
 * SlaveRemoved() is called immediately before a slave is removed.
 * NB: the associated slave window may have been destroyed when this
 * routine is called.
 */
typedef struct {			/* Manager hooks */
    Tk_GeomMgr tkGeomMgr;		/* "real" Tk Geometry Manager */
    Tk_OptionSpec *slaveOptionSpecs;	/* slave record options */
    size_t slaveSize;			/* size of slave record */

    int  (*RequestedSize)(void *managerData, int *widthPtr, int *heightPtr);
    void (*PlaceSlaves)(void *managerData);

    void (*SlaveAdded)(Ttk_Manager *, int slaveIndex);
    void (*SlaveRemoved)(Ttk_Manager *, int slaveIndex);
    int  (*SlaveConfigured)(
	    Tcl_Interp *, Ttk_Manager *, Ttk_Slave *, unsigned mask);
} Ttk_ManagerSpec;

/*
 * Default implementations for Tk_GeomMgr hooks:
 */
extern void Ttk_GeometryRequestProc(ClientData, Tk_Window slave);
extern void Ttk_LostSlaveProc(ClientData, Tk_Window slave);

struct TtkSlave_
{
    Tk_Window 		slaveWindow;
    Ttk_Manager 	*manager;
    void 		*slaveData;
    unsigned		flags;			/* private; see manager.c */
};

struct TtkManager_
{
    Ttk_ManagerSpec	*managerSpec;
    void 		*managerData;
    Tk_Window   	masterWindow;
    Tk_OptionTable 	slaveOptionTable;
    unsigned		flags;			/* private; see manager.c */
    int 	 	nSlaves;
    Ttk_Slave 		**slaves;
};

/*
 * Public API:
 */
extern Ttk_Manager *Ttk_CreateManager(
	Ttk_ManagerSpec *, void *managerData, Tk_Window masterWindow);
extern void Ttk_DeleteManager(Ttk_Manager *);

extern int Ttk_AddSlave(
    Tcl_Interp *, Ttk_Manager *, Tk_Window, int position,
    int objc, Tcl_Obj *CONST objv[]);

extern void Ttk_ForgetSlave(Ttk_Manager *, int slaveIndex);

extern int Ttk_ConfigureSlave(
    Tcl_Interp *interp, Ttk_Manager *, Ttk_Slave *,
    int objc, Tcl_Obj *CONST objv[]);

extern void Ttk_ReorderSlave(Ttk_Manager *, int fromIndex, int toIndex);
    /* Rearrange slave positions */

extern void Ttk_PlaceSlave(
    Ttk_Manager *, int slaveIndex, int x, int y, int width, int height);
    /* Position and map the slave */

extern void Ttk_UnmapSlave(Ttk_Manager *, int slaveIndex);
    /* Unmap the slave */

extern void Ttk_ManagerSizeChanged(Ttk_Manager *);
extern void Ttk_ManagerLayoutChanged(Ttk_Manager *);
    /* Notify manager that size (resp. layout) needs to be recomputed */

/* Utilities:
 */
extern int Ttk_SlaveIndex(Ttk_Manager *, Tk_Window);
    /* Returns: index in slave array of specified window, -1 if not found */

extern Ttk_Slave *Ttk_GetSlaveFromObj(
    Tcl_Interp *, Ttk_Manager *, Tcl_Obj *, int *indexPtr);

/* Accessor functions:
 */
extern int Ttk_NumberSlaves(Ttk_Manager *);
    /* Returns: number of managed slaves */

extern void *Ttk_SlaveData(Ttk_Manager *, int slaveIndex);
    /* Returns: private data associated with slave */

extern Tk_Window Ttk_SlaveWindow(Ttk_Manager *, int slaveIndex);
    /* Returns: slave window */

extern int Ttk_Maintainable(Tcl_Interp *, Tk_Window slave, Tk_Window master);
    /* Returns: 1 if master can manage slave; 0 otherwise leaving error msg */

#endif