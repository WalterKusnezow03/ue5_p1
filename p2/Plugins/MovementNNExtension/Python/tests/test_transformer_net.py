import unittest

import torch

from Net.NetTypes.TransformerNet import TransformerNet
from Base.Net.TensorInput.FTensor import FTensor


class TransformerNetTests(unittest.TestCase):
    def test_batch_preserves_token_order_and_prediction_layout(self):
        model = TransformerNet(2, 8, FTensor(20, 1, 4), FTensor(5, 1, 4))
        captured = []
        model.WQ.register_forward_pre_hook(lambda module, inputs: captured.append(inputs[0].detach()))
        with torch.no_grad():
            model.prediction.weight.zero_()
            model.prediction.bias.copy_(torch.arange(20, dtype=torch.float32))

        inputs = torch.arange(160, dtype=torch.float32).reshape(2, 4, 1, 20)
        output = model(inputs)

        expected_tokens = inputs.reshape(2, 20, 4) + model.pos_encoding[:20]
        torch.testing.assert_close(captured[0], expected_tokens)
        self.assertEqual(tuple(output.shape), (2, 4, 1, 5))
        torch.testing.assert_close(output.reshape(2, 20), torch.arange(20).float().repeat(2, 1))
        torch.testing.assert_close(model(inputs.reshape(2, 4, 20, 1)), output)
        torch.testing.assert_close(captured[1], expected_tokens)

    def test_forward_supports_training_and_rejects_wrong_shape(self):
        model = TransformerNet(2, 8, FTensor(20, 1, 4), FTensor(5, 1, 4))
        output = model(torch.randn(2, 4, 1, 20))
        output.square().mean().backward()
        self.assertIsNotNone(model.WQ.weight.grad)
        with self.assertRaises(ValueError):
            model(torch.randn(2, 4, 10, 2))
        with self.assertRaises(ValueError):
            model(torch.randn(2, 4, 20))


if __name__ == "__main__":
    unittest.main()
