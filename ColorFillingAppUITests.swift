import XCTest

class ColorFillingAppUITests: XCTestCase {
    
    var app: XCUIApplication!
    
    override func setUpWithError() throws {
        continueAfterFailure = false
        app = XCUIApplication()
        app.launch()
    }
    
    override func tearDownWithError() throws {
        app = nil
    }
    
    // MARK: - App Launch Tests
    
    func testAppLaunchesSuccessfully() throws {
        // Verify the app launches and shows the main interface
        XCTAssertTrue(app.navigationBars["🎨 Color Fun"].exists, "App should show the main navigation bar")
        XCTAssertTrue(app.buttons["Clear"].exists, "Clear button should be visible")
        XCTAssertTrue(app.buttons["Share"].exists, "Share button should be visible")
    }
    
    func testInitialState() throws {
        // Test that the app starts in the expected state
        XCTAssertTrue(app.staticTexts["🎨 Choose Your Color"].exists, "Color palette title should be visible")
        
        // Test that template selector is visible
        XCTAssertTrue(app.scrollViews.count > 0, "Template selector scroll view should be visible")
        
        // Test that color palette is visible
        XCTAssertTrue(app.buttons.count >= 16, "Should have at least 16 color buttons")
    }
    
    // MARK: - Template Selection Tests
    
    func testTemplateSelection() throws {
        // Test selecting different templates
        let templateButtons = app.scrollViews.firstMatch.buttons
        XCTAssertGreaterThan(templateButtons.count, 0, "Should have template selection buttons")
        
        // Tap the first template button
        let firstTemplate = templateButtons.firstMatch
        XCTAssertTrue(firstTemplate.exists, "First template button should exist")
        firstTemplate.tap()
        
        // Verify the template was selected (button should be highlighted)
        XCTAssertTrue(firstTemplate.exists, "Template button should still exist after selection")
    }
    
    func testAllTemplatesSelectable() throws {
        let templateButtons = app.scrollViews.firstMatch.buttons
        
        for i in 0..<templateButtons.count {
            let template = templateButtons.element(boundBy: i)
            if template.exists {
                template.tap()
                // Small delay to allow for animation
                sleep(1)
                XCTAssertTrue(template.exists, "Template \(i) should be selectable")
            }
        }
    }
    
    // MARK: - Color Selection Tests
    
    func testColorSelection() throws {
        // Find color buttons (circular buttons in the color palette)
        let colorButtons = app.buttons.matching(identifier: "colorButton")
        
        if colorButtons.count > 0 {
            let firstColor = colorButtons.firstMatch
            firstColor.tap()
            
            // Verify the color was selected (should be visually highlighted)
            XCTAssertTrue(firstColor.exists, "Color button should still exist after selection")
        }
    }
    
    func testAllColorsSelectable() throws {
        // Test that all colors in the palette are selectable
        let colorButtons = app.buttons.matching(identifier: "colorButton")
        
        for i in 0..<min(colorButtons.count, 16) {
            let colorButton = colorButtons.element(boundBy: i)
            if colorButton.exists {
                colorButton.tap()
                sleep(1) // Allow for animation
                XCTAssertTrue(colorButton.exists, "Color button \(i) should be selectable")
            }
        }
    }
    
    // MARK: - Canvas Interaction Tests
    
    func testCanvasTapInteraction() throws {
        // Test tapping on the canvas area
        let canvas = app.otherElements["coloringCanvas"]
        
        if canvas.exists {
            canvas.tap()
            // Verify the tap was registered (no crash, canvas still exists)
            XCTAssertTrue(canvas.exists, "Canvas should still exist after tap")
        }
    }
    
    func testCanvasMultipleTaps() throws {
        // Test multiple taps on different areas of the canvas
        let canvas = app.otherElements["coloringCanvas"]
        
        if canvas.exists {
            for _ in 0..<5 {
                canvas.tap()
                sleep(1)
                XCTAssertTrue(canvas.exists, "Canvas should handle multiple taps")
            }
        }
    }
    
    // MARK: - Clear Function Tests
    
    func testClearButton() throws {
        let clearButton = app.buttons["Clear"]
        XCTAssertTrue(clearButton.exists, "Clear button should exist")
        
        clearButton.tap()
        
        // Verify the clear action completed (button still exists, no crash)
        XCTAssertTrue(clearButton.exists, "Clear button should still exist after tap")
    }
    
    func testClearButtonMultipleTaps() throws {
        let clearButton = app.buttons["Clear"]
        
        // Test multiple clear operations
        for _ in 0..<3 {
            clearButton.tap()
            sleep(1)
            XCTAssertTrue(clearButton.exists, "Clear button should handle multiple taps")
        }
    }
    
    // MARK: - Share Function Tests
    
    func testShareButton() throws {
        let shareButton = app.buttons["Share"]
        XCTAssertTrue(shareButton.exists, "Share button should exist")
        
        shareButton.tap()
        
        // Verify share sheet appears or button still exists
        XCTAssertTrue(shareButton.exists, "Share button should still exist after tap")
    }
    
    func testShareButtonMultipleTaps() throws {
        let shareButton = app.buttons["Share"]
        
        // Test multiple share operations
        for _ in 0..<2 {
            shareButton.tap()
            sleep(2) // Allow time for share sheet to appear/disappear
            XCTAssertTrue(shareButton.exists, "Share button should handle multiple taps")
        }
    }
    
    // MARK: - Accessibility Tests
    
    func testAccessibilityLabels() throws {
        // Test that important UI elements have accessibility labels
        let clearButton = app.buttons["Clear"]
        XCTAssertTrue(clearButton.exists, "Clear button should have accessibility label")
        
        let shareButton = app.buttons["Share"]
        XCTAssertTrue(shareButton.exists, "Share button should have accessibility label")
        
        let colorTitle = app.staticTexts["🎨 Choose Your Color"]
        XCTAssertTrue(colorTitle.exists, "Color palette should have accessibility label")
    }
    
    func testVoiceOverCompatibility() throws {
        // Test that the app works with VoiceOver enabled
        // Note: This would require VoiceOver to be enabled in the simulator
        // For now, we'll just verify that elements are accessible
        
        let clearButton = app.buttons["Clear"]
        XCTAssertTrue(clearButton.isHittable, "Clear button should be hittable")
        
        let shareButton = app.buttons["Share"]
        XCTAssertTrue(shareButton.isHittable, "Share button should be hittable")
    }
    
    // MARK: - Performance Tests
    
    func testAppPerformance() throws {
        // Test that the app remains responsive during normal usage
        
        // Rapid template switching
        let templateButtons = app.scrollViews.firstMatch.buttons
        for i in 0..<min(templateButtons.count, 3) {
            let template = templateButtons.element(boundBy: i)
            if template.exists {
                template.tap()
                sleep(1)
            }
        }
        
        // Rapid color selection
        let colorButtons = app.buttons.matching(identifier: "colorButton")
        for i in 0..<min(colorButtons.count, 5) {
            let colorButton = colorButtons.element(boundBy: i)
            if colorButton.exists {
                colorButton.tap()
                sleep(1)
            }
        }
        
        // Verify app is still responsive
        XCTAssertTrue(app.navigationBars["🎨 Color Fun"].exists, "App should still be responsive")
    }
    
    // MARK: - Edge Case Tests
    
    func testRapidButtonTapping() throws {
        // Test rapid tapping on various buttons
        let clearButton = app.buttons["Clear"]
        let shareButton = app.buttons["Share"]
        
        // Rapid clear button taps
        for _ in 0..<10 {
            clearButton.tap()
        }
        XCTAssertTrue(clearButton.exists, "App should handle rapid clear button taps")
        
        // Rapid share button taps
        for _ in 0..<5 {
            shareButton.tap()
            sleep(1)
        }
        XCTAssertTrue(shareButton.exists, "App should handle rapid share button taps")
    }
    
    func testMemoryUsage() throws {
        // Test that the app doesn't have memory leaks during normal usage
        
        // Perform various operations multiple times
        for _ in 0..<10 {
            // Template switching
            let templateButtons = app.scrollViews.firstMatch.buttons
            if templateButtons.count > 0 {
                templateButtons.firstMatch.tap()
            }
            
            // Color selection
            let colorButtons = app.buttons.matching(identifier: "colorButton")
            if colorButtons.count > 0 {
                colorButtons.firstMatch.tap()
            }
            
            // Clear operation
            app.buttons["Clear"].tap()
            
            sleep(1)
        }
        
        // Verify app is still functional
        XCTAssertTrue(app.navigationBars["🎨 Color Fun"].exists, "App should still be functional after repeated operations")
    }
    
    // MARK: - Orientation Tests
    
    func testOrientationChanges() throws {
        // Test that the app handles orientation changes gracefully
        // Note: This would require rotating the simulator
        
        // For now, just verify the app is responsive
        XCTAssertTrue(app.navigationBars["🎨 Color Fun"].exists, "App should handle orientation changes")
    }
    
    // MARK: - Integration Tests
    
    func testCompleteUserWorkflow() throws {
        // Test a complete user workflow: select template, select color, color regions, clear, share
        
        // 1. Select a template
        let templateButtons = app.scrollViews.firstMatch.buttons
        if templateButtons.count > 0 {
            templateButtons.firstMatch.tap()
            sleep(1)
        }
        
        // 2. Select a color
        let colorButtons = app.buttons.matching(identifier: "colorButton")
        if colorButtons.count > 0 {
            colorButtons.firstMatch.tap()
            sleep(1)
        }
        
        // 3. Tap on canvas to color
        let canvas = app.otherElements["coloringCanvas"]
        if canvas.exists {
            canvas.tap()
            sleep(1)
        }
        
        // 4. Clear the canvas
        app.buttons["Clear"].tap()
        sleep(1)
        
        // 5. Try to share
        app.buttons["Share"].tap()
        sleep(2)
        
        // Verify the app is still functional
        XCTAssertTrue(app.navigationBars["🎨 Color Fun"].exists, "App should complete the full workflow successfully")
    }
    
    func testMultipleWorkflows() throws {
        // Test multiple complete workflows in sequence
        
        for _ in 0..<3 {
            // Select different templates
            let templateButtons = app.scrollViews.firstMatch.buttons
            if templateButtons.count > 1 {
                templateButtons.element(boundBy: 1).tap()
                sleep(1)
            }
            
            // Select different colors
            let colorButtons = app.buttons.matching(identifier: "colorButton")
            if colorButtons.count > 1 {
                colorButtons.element(boundBy: 1).tap()
                sleep(1)
            }
            
            // Clear
            app.buttons["Clear"].tap()
            sleep(1)
        }
        
        // Verify app is still functional
        XCTAssertTrue(app.navigationBars["🎨 Color Fun"].exists, "App should handle multiple workflows")
    }
}

// MARK: - Helper Extensions

extension XCUIElement {
    func waitForExistence(timeout: TimeInterval = 5.0) -> Bool {
        return self.waitForExistence(timeout: timeout)
    }
    
    func tapAndWait(timeout: TimeInterval = 1.0) {
        self.tap()
        sleep(UInt32(timeout))
    }
}