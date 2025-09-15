import SwiftUI
import UIKit

// MARK: - Error Types

enum ColoringAppError: LocalizedError {
    case invalidTemplate
    case invalidRegion
    case colorSelectionFailed
    case imageCaptureFailed
    case shareFailed
    case hapticFeedbackUnavailable
    case memoryWarning
    
    var errorDescription: String? {
        switch self {
        case .invalidTemplate:
            return "The selected template is not valid"
        case .invalidRegion:
            return "The region you're trying to color is not valid"
        case .colorSelectionFailed:
            return "Failed to select the color"
        case .imageCaptureFailed:
            return "Failed to capture your artwork"
        case .shareFailed:
            return "Failed to share your artwork"
        case .hapticFeedbackUnavailable:
            return "Haptic feedback is not available on this device"
        case .memoryWarning:
            return "The app is using too much memory"
        }
    }
    
    var recoverySuggestion: String? {
        switch self {
        case .invalidTemplate:
            return "Please select a different template"
        case .invalidRegion:
            return "Please try tapping a different area"
        case .colorSelectionFailed:
            return "Please try selecting the color again"
        case .imageCaptureFailed:
            return "Please try again or restart the app"
        case .shareFailed:
            return "Please check your device settings and try again"
        case .hapticFeedbackUnavailable:
            return "This feature is not available on your device"
        case .memoryWarning:
            return "Please clear some regions and try again"
        }
    }
}

// MARK: - Error Handler

class ErrorHandler: ObservableObject {
    @Published var currentError: ColoringAppError?
    @Published var showErrorAlert = false
    
    func handleError(_ error: ColoringAppError) {
        DispatchQueue.main.async {
            self.currentError = error
            self.showErrorAlert = true
        }
        
        // Log error for debugging
        print("ColoringApp Error: \(error.localizedDescription)")
    }
    
    func clearError() {
        currentError = nil
        showErrorAlert = false
    }
}

// MARK: - Safe Haptic Feedback

class SafeHapticFeedback {
    static let shared = SafeHapticFeedback()
    private var isAvailable: Bool
    
    private init() {
        // Check if haptic feedback is available
        if #available(iOS 13.0, *) {
            isAvailable = true
        } else {
            isAvailable = false
        }
    }
    
    func lightImpact() {
        guard isAvailable else { return }
        
        DispatchQueue.main.async {
            let impactFeedback = UIImpactFeedbackGenerator(style: .light)
            impactFeedback.impactOccurred()
        }
    }
    
    func mediumImpact() {
        guard isAvailable else { return }
        
        DispatchQueue.main.async {
            let impactFeedback = UIImpactFeedbackGenerator(style: .medium)
            impactFeedback.impactOccurred()
        }
    }
    
    func heavyImpact() {
        guard isAvailable else { return }
        
        DispatchQueue.main.async {
            let impactFeedback = UIImpactFeedbackGenerator(style: .heavy)
            impactFeedback.impactOccurred()
        }
    }
    
    func success() {
        guard isAvailable else { return }
        
        DispatchQueue.main.async {
            let notificationFeedback = UINotificationFeedbackGenerator()
            notificationFeedback.notificationOccurred(.success)
        }
    }
    
    func error() {
        guard isAvailable else { return }
        
        DispatchQueue.main.async {
            let notificationFeedback = UINotificationFeedbackGenerator()
            notificationFeedback.notificationOccurred(.error)
        }
    }
}

// MARK: - Safe Image Capture

class SafeImageCapture {
    static func captureImage(from view: some View, errorHandler: ErrorHandler) -> UIImage? {
        do {
            let renderer = ImageRenderer(content: view)
            
            // Set a reasonable size to prevent memory issues
            renderer.proposedSize = .init(width: 800, height: 800)
            
            guard let uiImage = renderer.uiImage else {
                throw ColoringAppError.imageCaptureFailed
            }
            
            // Check image size to prevent memory issues
            let imageSize = uiImage.size.width * uiImage.size.height
            if imageSize > 1000000 { // 1 megapixel limit
                throw ColoringAppError.memoryWarning
            }
            
            return uiImage
        } catch {
            errorHandler.handleError(.imageCaptureFailed)
            return nil
        }
    }
}

// MARK: - Memory Monitor

class MemoryMonitor: ObservableObject {
    @Published var memoryWarning = false
    private var timer: Timer?
    
    func startMonitoring() {
        timer = Timer.scheduledTimer(withTimeInterval: 5.0, repeats: true) { _ in
            self.checkMemoryUsage()
        }
    }
    
    func stopMonitoring() {
        timer?.invalidate()
        timer = nil
    }
    
    private func checkMemoryUsage() {
        let memoryInfo = mach_task_basic_info()
        var count = mach_msg_type_number_t(MemoryLayout<mach_task_basic_info>.size)/4
        
        let kerr: kern_return_t = withUnsafeMutablePointer(to: &memoryInfo) {
            $0.withMemoryRebound(to: integer_t.self, capacity: 1) {
                task_info(mach_task_self_,
                         task_flavor_t(MACH_TASK_BASIC_INFO),
                         $0,
                         &count)
            }
        }
        
        if kerr == KERN_SUCCESS {
            let memoryUsageMB = memoryInfo.resident_size / 1024 / 1024
            
            // Trigger warning if memory usage exceeds 100MB
            if memoryUsageMB > 100 {
                DispatchQueue.main.async {
                    self.memoryWarning = true
                }
            }
        }
    }
}

// MARK: - Input Validation

class InputValidator {
    static func validateTemplate(_ template: ColoringTemplate) -> Bool {
        // Check if template has valid regions
        guard !template.regions.isEmpty else { return false }
        
        // Check if all regions have valid IDs and paths
        for region in template.regions {
            guard !region.id.isEmpty else { return false }
            guard !region.path.isEmpty else { return false }
        }
        
        // Check if template has valid drawing path
        guard !template.drawing.isEmpty else { return false }
        
        return true
    }
    
    static func validateColor(_ color: Color) -> Bool {
        // Basic color validation - ensure color is not nil
        return true // SwiftUI Color is always valid
    }
    
    static func validateRegionId(_ regionId: String) -> Bool {
        // Check if region ID is valid
        guard !regionId.isEmpty else { return false }
        guard regionId.count <= 50 else { return false } // Reasonable length limit
        
        // Check for valid characters only
        let allowedCharacters = CharacterSet.alphanumerics
        let regionCharacters = CharacterSet(charactersIn: regionId)
        guard allowedCharacters.isSuperset(of: regionCharacters) else { return false }
        
        return true
    }
}

// MARK: - Safe Share Sheet

struct SafeShareSheet: UIViewControllerRepresentable {
    let activityItems: [Any]
    let errorHandler: ErrorHandler
    
    func makeUIViewController(context: Context) -> UIActivityViewController {
        let controller = UIActivityViewController(activityItems: activityItems, applicationActivities: nil)
        
        // Handle completion
        controller.completionWithItemsHandler = { activityType, completed, returnedItems, error in
            if let error = error {
                DispatchQueue.main.async {
                    self.errorHandler.handleError(.shareFailed)
                }
            } else if completed {
                SafeHapticFeedback.shared.success()
            }
        }
        
        return controller
    }
    
    func updateUIViewController(_ uiViewController: UIActivityViewController, context: Context) {}
}

// MARK: - Error Alert View

struct ErrorAlertView: View {
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        if let error = errorHandler.currentError {
            VStack {
                Image(systemName: "exclamationmark.triangle")
                    .font(.system(size: 50))
                    .foregroundColor(.orange)
                
                Text("Oops!")
                    .font(.title)
                    .fontWeight(.bold)
                
                Text(error.localizedDescription)
                    .multilineTextAlignment(.center)
                    .padding()
                
                if let suggestion = error.recoverySuggestion {
                    Text(suggestion)
                        .font(.caption)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                        .padding(.horizontal)
                }
                
                Button("OK") {
                    errorHandler.clearError()
                }
                .buttonStyle(.borderedProminent)
                .padding()
            }
            .padding()
            .background(Color.white)
            .cornerRadius(20)
            .shadow(radius: 10)
            .onAppear {
                SafeHapticFeedback.shared.error()
            }
        }
    }
}

// MARK: - Memory Warning View

struct MemoryWarningView: View {
    @ObservedObject var memoryMonitor: MemoryMonitor
    @ObservedObject var errorHandler: ErrorHandler
    
    var body: some View {
        if memoryMonitor.memoryWarning {
            VStack {
                Image(systemName: "memorychip")
                    .font(.system(size: 40))
                    .foregroundColor(.yellow)
                
                Text("Memory Warning")
                    .font(.headline)
                    .fontWeight(.bold)
                
                Text("The app is using a lot of memory. Consider clearing some regions.")
                    .multilineTextAlignment(.center)
                    .padding()
                
                Button("Clear All") {
                    // This would trigger a clear all action
                    memoryMonitor.memoryWarning = false
                }
                .buttonStyle(.borderedProminent)
                
                Button("Continue") {
                    memoryMonitor.memoryWarning = false
                }
                .buttonStyle(.bordered)
            }
            .padding()
            .background(Color.white)
            .cornerRadius(15)
            .shadow(radius: 5)
        }
    }
}