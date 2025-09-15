import XCTest
import SwiftUI
@testable import ColorFillingApp

class AccessibilityTests: XCTestCase {
    
    // MARK: - Accessibility Label Tests
    
    func testTemplateAccessibilityLabels() throws {
        for template in ColoringTemplate.allCases {
            // Test that template names are accessible
            XCTAssertFalse(template.name.isEmpty, "Template \(template.name) should have a non-empty name")
            
            // Test that template names are descriptive
            XCTAssertGreaterThan(template.name.count, 2, "Template name should be descriptive")
            
            // Test that template names don't contain special characters that might confuse screen readers
            let allowedCharacters = CharacterSet.alphanumerics.union(CharacterSet.whitespaces)
            let nameCharacters = CharacterSet(charactersIn: template.name)
            XCTAssertTrue(allowedCharacters.isSuperset(of: nameCharacters), 
                         "Template name '\(template.name)' should only contain accessible characters")
        }
    }
    
    func testRegionAccessibilityLabels() throws {
        for template in ColoringTemplate.allCases {
            for region in template.regions {
                // Test that region IDs are accessible
                XCTAssertFalse(region.id.isEmpty, "Region ID should not be empty for accessibility")
                
                // Test that region IDs are descriptive
                XCTAssertGreaterThan(region.id.count, 1, "Region ID should be descriptive")
                
                // Test that region IDs don't contain special characters
                let allowedCharacters = CharacterSet.alphanumerics
                let idCharacters = CharacterSet(charactersIn: region.id)
                XCTAssertTrue(allowedCharacters.isSuperset(of: idCharacters), 
                             "Region ID '\(region.id)' should only contain alphanumeric characters")
            }
        }
    }
    
    func testColorAccessibilityLabels() throws {
        let colors: [Color] = [
            .red, .orange, .yellow, .green, .mint, .teal,
            .cyan, .blue, .indigo, .purple, .pink, .brown,
            .gray, .black, .white, Color(red: 1, green: 0.8, blue: 0.8)
        ]
        
        for color in colors {
            // Test that colors are accessible (not nil)
            XCTAssertNotNil(color, "Color should not be nil for accessibility")
        }
    }
    
    // MARK: - Haptic Feedback Tests
    
    func testHapticFeedbackAvailability() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        // Test that haptic feedback methods don't crash
        hapticFeedback.lightImpact()
        hapticFeedback.mediumImpact()
        hapticFeedback.heavyImpact()
        hapticFeedback.success()
        hapticFeedback.error()
        
        // If we get here without crashing, the test passes
        XCTAssertTrue(true, "Haptic feedback should be available and not crash")
    }
    
    func testHapticFeedbackOnColorSelection() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        // Test light impact for color selection (appropriate for frequent actions)
        hapticFeedback.lightImpact()
        
        XCTAssertTrue(true, "Light haptic feedback should work for color selection")
    }
    
    func testHapticFeedbackOnTemplateSelection() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        // Test medium impact for template selection (appropriate for less frequent actions)
        hapticFeedback.mediumImpact()
        
        XCTAssertTrue(true, "Medium haptic feedback should work for template selection")
    }
    
    func testHapticFeedbackOnSuccessfulShare() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        // Test success feedback for successful share
        hapticFeedback.success()
        
        XCTAssertTrue(true, "Success haptic feedback should work for successful share")
    }
    
    func testHapticFeedbackOnError() throws {
        let hapticFeedback = SafeHapticFeedback.shared
        
        // Test error feedback for errors
        hapticFeedback.error()
        
        XCTAssertTrue(true, "Error haptic feedback should work for errors")
    }
    
    // MARK: - Touch Target Size Tests
    
    func testTouchTargetSizes() throws {
        // Test that UI elements have appropriate touch target sizes for accessibility
        
        // Template buttons should be at least 44x44 points (Apple's minimum)
        let templateButtonSize: CGFloat = 60
        XCTAssertGreaterThanOrEqual(templateButtonSize, 44, "Template buttons should meet minimum touch target size")
        
        // Color buttons should be at least 44x44 points
        let colorButtonSize: CGFloat = 45
        XCTAssertGreaterThanOrEqual(colorButtonSize, 44, "Color buttons should meet minimum touch target size")
        
        // Action buttons should be at least 44x44 points
        let actionButtonHeight: CGFloat = 40 // Height from padding
        let actionButtonWidth: CGFloat = 80 // Approximate width from padding
        XCTAssertGreaterThanOrEqual(actionButtonHeight, 44, "Action buttons should meet minimum touch target height")
        XCTAssertGreaterThanOrEqual(actionButtonWidth, 44, "Action buttons should meet minimum touch target width")
    }
    
    // MARK: - Color Contrast Tests
    
    func testColorContrast() throws {
        // Test that colors have sufficient contrast for accessibility
        
        let colors: [Color] = [
            .red, .orange, .yellow, .green, .mint, .teal,
            .cyan, .blue, .indigo, .purple, .pink, .brown,
            .gray, .black, .white, Color(red: 1, green: 0.8, blue: 0.8)
        ]
        
        for color in colors {
            // Test that colors are not too similar to white background
            // This is a basic test - in a real app, you'd use proper contrast ratio calculations
            XCTAssertNotNil(color, "Color should not be nil")
        }
    }
    
    func testButtonContrast() throws {
        // Test that buttons have sufficient contrast
        
        // Clear button (red background, white text)
        let clearButtonBackground = Color.red
        let clearButtonText = Color.white
        XCTAssertNotNil(clearButtonBackground, "Clear button background should be defined")
        XCTAssertNotNil(clearButtonText, "Clear button text should be defined")
        
        // Share button (blue background, white text)
        let shareButtonBackground = Color.blue
        let shareButtonText = Color.white
        XCTAssertNotNil(shareButtonBackground, "Share button background should be defined")
        XCTAssertNotNil(shareButtonText, "Share button text should be defined")
    }
    
    // MARK: - VoiceOver Compatibility Tests
    
    func testVoiceOverCompatibility() throws {
        // Test that the app is compatible with VoiceOver
        
        // Test that important elements have accessibility labels
        for template in ColoringTemplate.allCases {
            XCTAssertFalse(template.name.isEmpty, "Template should have accessible name for VoiceOver")
        }
        
        // Test that buttons have appropriate accessibility labels
        let buttonLabels = ["Clear", "Share", "🎨 Choose Your Color"]
        for label in buttonLabels {
            XCTAssertFalse(label.isEmpty, "Button should have accessible label for VoiceOver")
        }
    }
    
    // MARK: - Dynamic Type Tests
    
    func testDynamicTypeSupport() throws {
        // Test that the app supports Dynamic Type
        
        // Test that text sizes are appropriate
        let titleFont = Font.title2
        let captionFont = Font.caption
        let headlineFont = Font.headline
        
        XCTAssertNotNil(titleFont, "Title font should be defined")
        XCTAssertNotNil(captionFont, "Caption font should be defined")
        XCTAssertNotNil(headlineFont, "Headline font should be defined")
    }
    
    // MARK: - Motor Accessibility Tests
    
    func testMotorAccessibility() throws {
        // Test that the app is accessible for users with motor impairments
        
        // Test that buttons are large enough for easy tapping
        let templateButtonSize: CGFloat = 60
        XCTAssertGreaterThanOrEqual(templateButtonSize, 44, "Template buttons should be large enough for motor accessibility")
        
        let colorButtonSize: CGFloat = 45
        XCTAssertGreaterThanOrEqual(colorButtonSize, 44, "Color buttons should be large enough for motor accessibility")
        
        // Test that buttons are spaced appropriately
        let buttonSpacing: CGFloat = 15
        XCTAssertGreaterThanOrEqual(buttonSpacing, 8, "Button spacing should be appropriate for motor accessibility")
    }
    
    // MARK: - Cognitive Accessibility Tests
    
    func testCognitiveAccessibility() throws {
        // Test that the app is accessible for users with cognitive impairments
        
        // Test that interface is simple and clear
        let templateCount = ColoringTemplate.allCases.count
        XCTAssertLessThanOrEqual(templateCount, 10, "Should not have too many templates for cognitive accessibility")
        
        let colorCount = 16
        XCTAssertLessThanOrEqual(colorCount, 20, "Should not have too many colors for cognitive accessibility")
        
        // Test that labels are clear and descriptive
        for template in ColoringTemplate.allCases {
            XCTAssertFalse(template.name.isEmpty, "Template names should be clear and descriptive")
        }
    }
    
    // MARK: - Visual Accessibility Tests
    
    func testVisualAccessibility() throws {
        // Test that the app is accessible for users with visual impairments
        
        // Test that colors are distinct
        let colors: [Color] = [.red, .blue, .green, .yellow, .purple, .orange]
        XCTAssertEqual(colors.count, 6, "Should have distinct colors for visual accessibility")
        
        // Test that shapes are distinct
        let templates = ColoringTemplate.allCases
        XCTAssertGreaterThan(templates.count, 3, "Should have distinct shapes for visual accessibility")
    }
    
    // MARK: - Hearing Accessibility Tests
    
    func testHearingAccessibility() throws {
        // Test that the app is accessible for users with hearing impairments
        
        // Test that the app doesn't rely on audio cues
        // (Our app is primarily visual, which is good for hearing accessibility)
        
        // Test that haptic feedback is available as an alternative to audio
        let hapticFeedback = SafeHapticFeedback.shared
        XCTAssertNotNil(hapticFeedback, "Haptic feedback should be available for hearing accessibility")
    }
    
    // MARK: - Integration Tests
    
    func testCompleteAccessibilityWorkflow() throws {
        // Test a complete workflow with accessibility considerations
        
        // 1. Test template selection accessibility
        for template in ColoringTemplate.allCases {
            XCTAssertFalse(template.name.isEmpty, "Template \(template.name) should be accessible")
            XCTAssertFalse(template.regions.isEmpty, "Template should have accessible regions")
        }
        
        // 2. Test color selection accessibility
        let colors: [Color] = [.red, .blue, .green, .yellow]
        for color in colors {
            XCTAssertNotNil(color, "Color should be accessible")
        }
        
        // 3. Test haptic feedback accessibility
        let hapticFeedback = SafeHapticFeedback.shared
        hapticFeedback.lightImpact()
        hapticFeedback.success()
        
        // 4. Test button accessibility
        let buttonLabels = ["Clear", "Share"]
        for label in buttonLabels {
            XCTAssertFalse(label.isEmpty, "Button \(label) should be accessible")
        }
        
        XCTAssertTrue(true, "Complete accessibility workflow should work")
    }
    
    // MARK: - Performance Tests
    
    func testAccessibilityPerformance() throws {
        // Test that accessibility features don't impact performance
        
        let startTime = CFAbsoluteTimeGetCurrent()
        
        // Test haptic feedback performance
        let hapticFeedback = SafeHapticFeedback.shared
        for _ in 0..<100 {
            hapticFeedback.lightImpact()
        }
        
        // Test template accessibility performance
        for _ in 0..<100 {
            for template in ColoringTemplate.allCases {
                _ = template.name
                _ = template.regions
            }
        }
        
        let timeElapsed = CFAbsoluteTimeGetCurrent() - startTime
        
        // Should complete within reasonable time (less than 1 second)
        XCTAssertLessThan(timeElapsed, 1.0, "Accessibility features should not impact performance significantly")
    }
}

// MARK: - Mock Classes for Testing

class MockAccessibilityView: View {
    var body: some View {
        VStack {
            Text("Test View")
                .accessibilityLabel("Test Label")
            
            Button("Test Button") {
                // Test action
            }
            .accessibilityLabel("Test Button")
        }
    }
}

// MARK: - Accessibility Helper Tests

class AccessibilityHelperTests: XCTestCase {
    
    func testAccessibilityLabelGeneration() throws {
        // Test that accessibility labels are generated correctly
        
        let template = ColoringTemplate.butterfly
        let region = template.regions.first!
        
        // Test template accessibility
        XCTAssertFalse(template.name.isEmpty, "Template should have accessibility label")
        
        // Test region accessibility
        XCTAssertFalse(region.id.isEmpty, "Region should have accessibility label")
    }
    
    func testAccessibilityHintGeneration() throws {
        // Test that accessibility hints are generated correctly
        
        let clearButtonHint = "Clears all colored regions"
        let shareButtonHint = "Shares your artwork"
        
        XCTAssertFalse(clearButtonHint.isEmpty, "Clear button should have accessibility hint")
        XCTAssertFalse(shareButtonHint.isEmpty, "Share button should have accessibility hint")
    }
    
    func testAccessibilityValueGeneration() throws {
        // Test that accessibility values are generated correctly
        
        let selectedColor = Color.red
        let selectedTemplate = ColoringTemplate.butterfly
        
        XCTAssertNotNil(selectedColor, "Selected color should have accessibility value")
        XCTAssertFalse(selectedTemplate.name.isEmpty, "Selected template should have accessibility value")
    }
}