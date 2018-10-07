//
//  AppDelegate.m
//  KVTree
//
//  Created by deng you hua on 2018/10/7.
//  Copyright © 2018 CC | ccworld1000@gmail.com. All rights reserved.
//

#import "AppDelegate.h"

#include <string.h>
#include <stdlib.h>

#import "kvtree.h"

#define TEST_PASS (0)
#define TEST_FAIL (1)

int kvtree_kv(){
    int rc = TEST_PASS;
    char* key = "key";
    char* value = "value";
    
    kvtree* kvt = kvtree_new();
    if (kvt == NULL) rc = TEST_FAIL;
    
    kvtree* val = kvtree_set_kv(kvt, key, value);
    if (val == NULL) rc = TEST_FAIL;
    char* valstr = kvtree_elem_get_first_val(kvt, key);
    if(strcmp(valstr, value)) rc = TEST_FAIL;
    
    int size = kvtree_size(kvt);
    if (size != 1) rc = TEST_FAIL;
    

    kvtree* get = kvtree_get_kv(kvt, key, value);
    if (get != val) rc = TEST_FAIL;
    
    printf("[\n");
    kvtree_print(kvt, 6);
    printf("]\n");
    
    if(kvtree_unset_kv(kvt, key, value) != KVTREE_SUCCESS) rc = TEST_FAIL;
    size = kvtree_size(kvt);
    if(size != 0) rc = TEST_FAIL;
    
    kvtree_delete(&kvt);
    return rc;
}

@interface AppDelegate ()

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    NSLog(@"test return = %d", kvtree_kv());
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
