
// Complex scope analysis for semantic coverage
function complexScopeAnalysis(): void {
    // Multiple nested scopes
    let level1 = "level1";
    
    function level2Function() {
        let level2 = "level2";
        
        function level3Function() {
            let level3 = "level3";
            
            function level4Function() {
                let level4 = "level4";
                
                function level5Function() {
                    let level5 = "level5";
                    console.log(level1, level2, level3, level4, level5);
                }
                
                level5Function();
            }
            
            level4Function();
        }
        
        level3Function();
    }
    
    level2Function();
    
    // Complex variable shadowing
    let shadowed = "outer";
    
    function shadowingFunction() {
        let shadowed = "inner";
        
        function innerShadowingFunction() {
            let shadowed = "innermost";
            console.log(shadowed);
        }
        
        innerShadowingFunction();
    }
    
    shadowingFunction();
}
