import SwiftUI
import UIKit

struct ColoringCanvasView: View {
    let template: ColoringTemplate
    @Binding var selectedColor: Color
    @ObservedObject var errorHandler: ErrorHandler
    @State private var coloredRegions: [String: Color] = [:]
    @State private var showShareSheet = false
    @State private var capturedImage: UIImage?
    
    var body: some View {
        GeometryReader { geometry in
            ZStack {
                // Background
                RoundedRectangle(cornerRadius: 20)
                    .fill(Color.white)
                    .shadow(color: .black.opacity(0.1), radius: 5, x: 0, y: 2)
                
                VStack {
                    // Template drawing with colored regions
                    ZStack {
                        // Colored regions (behind the outline)
                        ForEach(template.regions, id: \.id) { region in
                            region.path
                                .fill(coloredRegions[region.id] ?? Color.clear)
                                .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                        .onTapGesture {
                            // Validate region before coloring
                            guard InputValidator.validateRegionId(region.id) else {
                                errorHandler.handleError(.invalidRegion)
                                return
                            }
                            
                            withAnimation(.spring(response: 0.3, dampingFraction: 0.6)) {
                                coloredRegions[region.id] = selectedColor
                            }
                            
                            // Safe haptic feedback
                            SafeHapticFeedback.shared.lightImpact()
                        }
                        }
                        
                        // Template outline (on top)
                        template.drawing
                            .stroke(Color.black, lineWidth: 3)
                            .scaleEffect(min(geometry.size.width / 400, geometry.size.height / 400))
                    }
                    .frame(maxWidth: .infinity, maxHeight: .infinity)
                    .background(Color.white)
                    .cornerRadius(15)
                    .padding()
                    
                    // Action buttons
                    HStack(spacing: 20) {
                        // Clear button
                        Button(action: {
                            withAnimation(.spring()) {
                                coloredRegions.removeAll()
                            }
                            SafeHapticFeedback.shared.mediumImpact()
                        }) {
                            HStack {
                                Image(systemName: "trash")
                                Text("Clear")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 20)
                            .padding(.vertical, 10)
                            .background(Color.red)
                            .cornerRadius(20)
                        }
                        
                        // Save/Share button
                        Button(action: {
                            guard let image = captureImage() else {
                                errorHandler.handleError(.imageCaptureFailed)
                                return
                            }
                            capturedImage = image
                            showShareSheet = true
                        }) {
                            HStack {
                                Image(systemName: "square.and.arrow.up")
                                Text("Share")
                            }
                            .foregroundColor(.white)
                            .padding(.horizontal, 20)
                            .padding(.vertical, 10)
                            .background(Color.blue)
                            .cornerRadius(20)
                        }
                    }
                    .padding(.bottom, 10)
                }
            }
        }
        .onChange(of: template) { _ in
            withAnimation(.easeInOut) {
                coloredRegions.removeAll()
            }
        }
        .sheet(isPresented: $showShareSheet) {
            if let image = capturedImage {
                SafeShareSheet(activityItems: [image], errorHandler: errorHandler)
            }
        }
    }
    
    private func captureImage() -> UIImage? {
        let content = ZStack {
            Color.white
            VStack {
                ZStack {
                    // Colored regions
                    ForEach(template.regions, id: \.id) { region in
                        region.path
                            .fill(coloredRegions[region.id] ?? Color.clear)
                    }
                    
                    // Template outline
                    template.drawing
                        .stroke(Color.black, lineWidth: 3)
                }
                .frame(width: 400, height: 400)
            }
        }
        
        return SafeImageCapture.captureImage(from: content, errorHandler: errorHandler)
    }
}

struct ShareSheet: UIViewControllerRepresentable {
    let activityItems: [Any]
    
    func makeUIViewController(context: Context) -> UIActivityViewController {
        UIActivityViewController(activityItems: activityItems, applicationActivities: nil)
    }
    
    func updateUIViewController(_ uiViewController: UIActivityViewController, context: Context) {}
}

#Preview {
    ColoringCanvasView(template: .butterfly, selectedColor: .constant(.red))
        .frame(height: 500)
}